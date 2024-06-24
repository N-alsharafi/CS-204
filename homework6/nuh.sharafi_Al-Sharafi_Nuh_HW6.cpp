//
// Created by nuh al-sharafi on 5/16/24.
// This homework was developed and tested on Debian-12 (it ran all the test cases successfully).
// Results may or may not be replicable on windows.
//

//library imports
#include<iostream>
#include<mutex>
#include<thread>
#include<time.h>
#include<random>
#include<string>
#include<iomanip>

using namespace std;

//global variables
mutex mMutex;       //main mutex
mutex cMutex;       //output tidiness mutex

int targetNumber;   //number to be guessed
int roundWinner;    //stores id of winning thread
int currentRound;   //shows current round
int finalRound;     //shows total number of rounds
int lower;          //lower bound for random generation
int upper;          //upper bound for random generation


//following function was copied from lab 12.
// Generate random numbers in a thread-safe manner in the range  (min, max).
int random_range(const int & min, const int & max) {
    static mt19937 generator(time(0));
    uniform_int_distribution<int> distribution(min,max);
    return distribution(generator);
}


void get_input(int& noPlayers, int& noRounds, int& lowerBound, int& upperBound)
{
    /* function gets the input from the user, performs checks and returns the values via reference */

    //get number of players
    int players=0;
    cout << "Please enter number of players\n";
    cin >> players;

    while(players < 1)
    {
        cout << "Number of players cannot be lower than 1!\n";
        cout << "Please enter number of players\n";
        cin >> players;
    }
    noPlayers = players;

    //get number of rounds
    int rounds=0;
    cout << "Please enter number of rounds\n";
    cin >> rounds;

    while(rounds < 1)
    {
        cout << "Number of rounds cannot be lower than 1!\n";
        cout << "Please enter number of rounds\n";
        cin >> rounds;
    }
    noRounds = rounds;

    //get randomization range
    int lb=0, ub=0;
    cout << "Please enter the randomization range\n";
    cin >> lb >> ub;

    while (ub < lb)
    {
        cout << "Lower bound has to be smaller than or equal to higher bound\n";
        cout << "Please enter the randomization range\n";
        cin >> lb >> ub;
    }
    lowerBound = lb;
    upperBound = ub;
}


//player function
void player(int ID, struct tm *timeToStart)
{
    /* Function will simulate players, will start by sleeping until timeToStart is reached
     * will produce one random number within the range every second until the game ends
     * if the guess is identical, player will store his ID in roundWinner for host to handle
     * */

    //sleep_until
    this_thread::sleep_until(chrono::system_clock::from_time_t(mktime(timeToStart)));


    //make run until game ends
    /* we need to protect reads from checking a value mid-update, so we lock reads */
    mMutex.lock();
    while(currentRound <= finalRound)
    {
        mMutex.unlock();

        //guess
        int guess = random_range(lower, upper);

        //generate time
        mMutex.lock(); //we need to do the whole thing as soon as we generate time, can't context switch
        time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now());
        struct tm *ptm = localtime(&tt);

        //compare

        if(guess == targetNumber)
        {
            //we need to stop correct guesses once a winner is found
            if(roundWinner == -1)
            //implying roundWinner hasn't been set by someone else
            {
                roundWinner = ID;

                cMutex.lock();
                cout << "Player with id " << ID << " guessed " << guess << " correctly at: " << put_time(ptm, "%X")
                     << endl;
                cMutex.unlock();
                mMutex.unlock();
                // the cout statement being in the main mutex is crucial for the ordering of the output
                // because we can't have the program context switch and output another be processed before this
            }
            else
            {
                mMutex.unlock();
            }
        }
        else
        {
            //we want to stop the guessing once a winner is found
            if(roundWinner == -1)
            {
                cMutex.lock();
                cout << "Player with id " << ID << " guessed " << guess << " incorrectly at: " << put_time(ptm, "%X")
                     << endl;
                cMutex.unlock();
                mMutex.unlock(); //this output stream should also be within the same context for time purposes.
            }
            else
            {
                mMutex.unlock();
            }
        }

        //sleep
        this_thread::sleep_for(chrono::seconds(1));
        mMutex.lock();
    }
    //unlocking mutex after exiting the loop;
    mMutex.unlock();

}


//host function
void host(int* scoreB)
{
    /* This will simulate the host, it takes the round number to measure how long the game will last
     * function handles generating the random target each round, printing random target, handling round announcements
     * also keeping watch on roundWinner and tracking scores
     * */

    //game loop
    while(currentRound <= finalRound)
    {

        //round announcement
        if(currentRound != 1)
        //because first round is handled outside the thread
        {
            //generate time
            time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now());
            struct tm *ptm = localtime(&tt);

            //print message
            cMutex.lock();
            cout << "\n\n---------------------------------------------------\n";
            cout << "Round " << currentRound << " started at: " << put_time(ptm, "%X") << endl;
            cMutex.unlock();
        }

        //target generation & winner reset
        mMutex.lock();
        targetNumber = random_range(lower, upper);
        roundWinner = -1; //because indexes start from 0, -1 means there's no winner yet
        mMutex.unlock();

        //output target
        cMutex.lock();
        cout << "Target is " << targetNumber << endl << endl;
        cMutex.unlock();

        //watch for winner
        bool flag= true;
        while(flag)
        {
            mMutex.lock();
            if(roundWinner == -1)
            {
                mMutex.unlock();
                //this_thread::sleep_for(chrono::nanoseconds(1));
                //the problem with waiting is that it takes time which results in a bug
                //the issue initially was that there was nothing to put in the while loop
            }
            else
            {
                mMutex.unlock();
                flag = false;
            }


        }

        //update score and increment round
        scoreB[roundWinner]++;
        mMutex.lock();
        currentRound++;
        mMutex.unlock();

    }

    //make sure to reset the winner so that the correctGuess mutex is unlocked in the end
    cMutex.unlock(); //have to make sure we unlock at the end of the game
    mMutex.lock();
    roundWinner = -1;
    mMutex.unlock();

}


//main function
int main()
{

    //get inputs
    int numPlayers;
    get_input(numPlayers, finalRound,lower, upper);

    //initialize score array
    int* scoreboard = new int[numPlayers];

    //set scores to 0
    for(int i=0; i<numPlayers; i++)
    {
        scoreboard[i] = 0;
    }

    //set start time
    time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now());
    struct tm *ptm = localtime(&tt);
    cout << "\n\n---------------------------------------------------\n";
    cout << "Game started at: " << put_time(ptm, "%X") << endl;
    cout << "Round 1 will start 3 seconds later\n\n";
    currentRound = 1;

    ptm->tm_sec = ptm->tm_sec + 3; //increment time by 3 seconds

    //create & start threads ------------------->threading starts here
    thread hostThread(&host, scoreboard);
    thread playerThreads[numPlayers];

    for(int i=0; i< numPlayers; i++)
    {
        playerThreads[i] = (thread(&player, i, ptm));
    }

    //join threads
    hostThread.join();

    for(int i=0; i<numPlayers; i++)
    {
        playerThreads[i].join();
    }

    //show scores --------------------------------> threading ended
    cout << "\nGame is over!\n";
    cout << "Leaderboard:\n";

    for(int i=0; i<numPlayers; i++)
    {
        cout << "Player " << i << " has won " << scoreboard[i] << " times\n";
    }

    //delete dynamically allocated memory
    delete[] scoreboard;

    return 0;
}