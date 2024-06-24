//
// Created by nuh alsharafi on 5/12/24.
//

#include "Board.h"
#include <iostream>
using namespace std;


//Constructor
Board::Board(int size_i, int N_i)
: size(size_i), N(N_i)
{
    /* Function uses size to dynamically initialize a 2D matrix
     * of dimensions size * size, also sets all entries to '-'
     * also function sets privates variables */

    //initialize orientation to normal (it's arbitrary)
    orientation = 0;

    //create and initialize matrix
    board = new char* [size];
    for (int i=0; i< size; i++)
    {
        board[i] = new char[size];
        for(int j=0; j<size; j++)
        //initialize
        {
            board[i][j] = '-';
        }
    }
}


//Destructor
Board::~Board()
{
    /* Function deallocates the dynamically allocated matrix */
    for (int i = 0; i < size; i++)
    {
        delete[] board[i];
    }
    delete[] board;
}


void Board::print_board() const
{
    /* Function prints board based on orientation */
    cout << endl;
    if(orientation == 0)
    {
        //first orientation
        for(int i=0;i<size;i++)
        {
            for(int j=0;j<size;j++)
            {
                cout << board[i][j];
            }
            cout << endl;
        }
    }
    else if(orientation == 1)
    {
        //second orientation (90 degrees clockwise)
        for(int i=0;i<size;i++)
        {
            for(int j=size-1;j>=0;j--)
            {
                cout << board[j][i];
            }
            cout << endl;
        }
    }
    else if(orientation == 2)
    {
        //third orientation (180 degrees clockwise)
        for(int i=size-1;i>=0;i--)
        {
            for(int j=size-1;j>=0;j--)
            {
                cout << board[i][j];
            }
            cout << endl;
        }
    }
    else if(orientation == 3)
    {
        //fourth orientation (270 degrees clockwise)
        for(int i=size-1;i>=0;i--)
        {
            for(int j=0;j<size;j++)
            {
                cout << board[j][i];
            }
            cout << endl;
        }
    }
    else
    {
        cout << "ERROR: invalid orientation, not in range [0,3]\n";
    }
    cout << endl;
}


//put_piece
bool Board::put_piece(char piece, int column)
{
    /* Function performs checks and inserts piece into matrix */

    //range check
    if (column < 0 || column >=size)
    {
        //invalid column index
        return false;
    }

    //column still has space (depends on orientation)
    if (orientation == 0)
    {
        //check if column top spot is empty
        if (board[0][column] != '-')
        {
            //column does not have space
            return false;
        }
        else
        {
            board[0][column] = piece;
        }
    }
    else if (orientation == 1)
    {
        //check if column top spot is empty
        if (board[size-1-column][0] != '-')
        {
            return false; //no space
        }
        else
        {
            board[size-1-column][0] = piece;
        }
    }
    else if (orientation == 2)
    {
        //check with respect to orientation
        if (board[size-1][size-1-column] != '-')
        {
            return false;
        }
        else
        {
            board[size-1][size-1-column] = piece;
        }
    }
    else
    {
        //orientation == 3
        if(board[column][size-1] != '-')
        {
            return false;
        }
        else
        {
            board[column][size-1] = piece;
        }
    }

    //apply gravity
    gravity();
    return true;
}


//turn_board_clockwise
void Board::turn_board_clockwise()
{
    /* Function turns board clockwise by 90 degrees,
     * or makes the class deal with the object as if that happened */
    orientation = (++orientation) % 4;
    //shouldn't need the brackets to be there, but just in case.

    //apply gravity
    gravity();
}


//get_game_status
char Board::get_game_status()
{
    /* Function checks if there has been a winner
     * It's practically doing an efficient brute-force of the problem
     * what happens is that a smaller matrix is selected, it contains the possible
     * starting points of the winning sequence, then every entry in that matrix
     * is examined to see if it has a winning sequence
     * First the rows are checked, Then columns, then diagonals
     * and reverse diagonals. The orientation of the board does not matter
     * in this case.
     * */

    char winner = '-';

    //check row win
    for (int i =0; i<size; i++)
    //loop over every row
    {
        bool connected;
        for (int j=0; j<size-N+1; j++)
        //go through the row and check if there's a winning sequence
        {
            connected = true;
            for(int k=0; k<N; k++)
            {
                if (board[i][j] != board[i][j+k] || board[i][j] == '-') connected = false;
            }

            if(connected && winner == '-')
            {
                winner = board[i][j];
            }
            else if(connected && winner != board[i][j])
            {
                //this means there's already a winner, so it becomes a draw
                winner = 'D';
            }
        }
    }


    //check column win
    for (int i=0; i<size; i++)
    //loop over every column
    {
        bool connected;
        for(int j=0; j<size-N+1; j++)
        //go through each row
        {
            connected = true;
            for(int k=0;k<N;k++)
            {
                if(board[j][i] != board[j+k][i] || board[j][i] == '-') connected = false;
            }

            if(connected && winner == '-')
            {
                winner = board[j][i];
            }
            else if(connected && winner != board[j][i])
            {
                winner = 'D';
            }
        }
    }


    //check diagonals
    for(int i=0; i<size-N+1; i++)
    //loop over rows
    {
        bool connected;
        for(int j=0; j<size-N+1; j++)
        //loop over columns, we need both to loop independently
        {
            connected = true;
            for(int k=0;k<N;k++)
            //look for sequence
            {
                if(board[i][j] != board[i+k][j+k] || board[i][j] == '-') connected=false;
            }

            if (connected && winner == '-')
            {
                winner = board[i][j];
            }
            else if(connected && winner != board[i][j])
            {
                winner = 'D';
            }
        }
    }


    //check reverse diagonals
    for(int i=size-N;i>=0;i--)
    //loop over rows
    {
        bool connected;
        for(int j=size-1;j>N-2;j--)
        //loop over columns
        {
            connected = true;
            for (int k = 0; k < N; k++) {
                if (board[i][j] != board[i + k][j - k] || board[i][j] == '-') connected = false;
            }

            if (connected && winner == '-')
            {
                winner = board[i][j];
            }
            else if (connected && winner != board[i][j])
            {
                winner = 'D';
            }
        }
    }


    //check full board
    bool fullboard = true;
    for (int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            if(board[i][j] == '-') fullboard = false;
        }
    }

    if (fullboard)
    {
        winner = 'D';
    }
    return winner;
}


//gravity
void Board::gravity()
{
    /* The private function responsible for simulating gravity in the game,
     * is called everytime gravity needs to act, and depending on
     * the orientation, it pushes elements in a certain direction */

    if(orientation == 0)
    {
        for(int i=size-1;i>=0;i--)
        //for every row starting from the end
        {
            for(int j=0;j<size;j++)
            //loop through the columns of said row
            {
                //check if any element is a '-'
                if(board[i][j] == '-')
                {
                    //if an element is '-', check the elements above, swap the first non '-' element
                    for(int k=i-1;k>=0;k--)
                    {
                        //compare every element above dash element
                        if(board[k][j] != '-' && board[i][j] == '-')
                        //second condition is to make sure only the first element is swapped
                        {
                            //found a non '-' element, going to swap
                            board[i][j] = board[k][j];
                            board[k][j] = '-';  //shouldn't cause a bug because of the
                                                //if condition
                        }
                    }
                }
            }
        }
    }
    else if(orientation == 1)
    {
        for(int i=size-1;i>=0;i--)
        //for every column
        {
            for(int j=0;j<size;j++)
            //for every row
            {
                //check if element is a '-'
                if(board[j][i] == '-')
                {
                    //element is '-', check if there's an element to swap
                    for(int k=i-1;k>=0;k--)
                    {
                        //compare every element 'above' dash
                        if(board[j][k] != '-' && board[j][i] == '-')
                        {
                            //swap
                            board[j][i] = board[j][k];
                            board[j][k] = '-';
                        }
                    }
                }
            }
        }
    }
    else if(orientation == 2)
    {
        for(int i=0; i<size;i++)
        //for every row
        {
            for(int j=size-1;j>=0;j--)
            //for every column
            {
                //check if element is a '-'
                if(board[i][j] == '-')
                {
                    //check if there's an element to swap
                    for(int k=i+1;k<size;k++)
                    {
                        //compare the element and every one 'above'
                        if(board[k][j] != '-' && board[i][j] == '-')
                        {
                            //swap
                            board[i][j] = board[k][j];
                            board[k][j] = '-';
                        }
                    }
                }
            }
        }
    }
    else
    {
        //orientation = 3
        for(int i=0; i<size; i++)
        //for every column
        {
            for(int j=0; j<size; j++)
            //for every row
            {
                //check
                if(board[j][i] == '-')
                {
                    //check swap
                    for(int k=i+1; k<size; k++)
                    {
                        //compare
                        if(board[j][k] != '-' && board[j][i] == '-')
                        {
                            //swap
                            board[j][i] = board[j][k];
                            board[j][k] = '-';
                        }
                    }
                }
            }
        }
    }
}