//
// Created by Nuh Al-Sharafi on 3/13/2024.
//
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;


//struct definitions

struct NodeLike
{
    string subject;
    string object;
    NodeLike *next;
};

struct NodeTaken
{
    string name;
    NodeTaken *next;
};


//function declarations

//Processing functions; implemented.
void insertOneLikes(string& sub, string& obj, NodeLike* &headLikes);
void insertOneTaken(string& name, NodeTaken* &headTaken);
void deleteOneLikes(string& sub, string& obj, NodeLike * &headLikes);
void deleteLikesAll(NodeLike* headLikes);
void deleteTakenAll(NodeTaken* headTaken);
bool inLikes(const string& sub, const string& obj, NodeLike* headLikes);
bool inTaken(string& name, NodeTaken* headTaken);
void printLikes(NodeLike* headLikes);
void printTaken(NodeTaken* headTaken);

//high-level processing functions
void processLine(string& line, NodeLike* &headLikes, NodeTaken* &headTaken);
void parseLine(string& sub, string& obj, string& line);



int main() {

    //open the input file
    ifstream file;
    string input;

    cout << "Please enter the file name:\n";
    cin >> input;

    file.open(input.c_str());

    while(file.fail()) {
        cout << "Could not open the file. Please enter a valid file name:\n";
        cin >> input;
        file.open(input.c_str());
    }

    //loop over the file and process it
    NodeLike* headLikes = NULL;
    NodeTaken* headTaken = NULL;
    string line;
    int counter = 1;
    while(getline(file, line))
    {
        cout << "Read line number (" << counter << "): " << line << endl;
        counter++;

        processLine(line, headLikes, headTaken);
        printLikes(headLikes);
        printTaken(headTaken);
    }


    //clean up and exit
    deleteLikesAll(headLikes);
    deleteTakenAll(headTaken);
    cout << "Lists are deleted and the program ends successfully.\n";

    return 0;
}


//Implementation of high-level processing functions
void processLine(string& line, NodeLike* &headLikes, NodeTaken* &headTaken)
{
    /*This function will handle the input and what to do
     * */

    //parse input into subject and object
    string subject, object;
    parseLine(subject, object, line);

    bool case0 = inTaken(subject, headTaken) || inTaken(object, headTaken);
    bool case1 = inLikes(subject, object, headLikes);
    //there's a bug here
    bool case2 = inLikes(object, subject, headLikes);

    //check the 5 cases detailed in the homework doc
    if (case0)
    {
        //do nothing
        cout << "Either " << subject << " or " << object << " or both is/are already taken, so the like relation is not added.\n";
    }
    else if (case1)
    {
        //redundant so don't add
        cout << line << " relation already exists in the likes list, so it is not added to it.\n";
    }
    else if (case2)
    {
        //remove any pair containing subject or object in likes linked list and add both names to taken
        cout << "Match found: " << line << " and " << object << " likes " << subject << ".\n";

        //add both to taken list
        cout << "Added to taken list: " << subject << endl;
        insertOneTaken(subject, headTaken);
        cout << "Added to taken list: " << object << endl;
        insertOneTaken(object, headTaken);

        //remove both from any node containing them in the likes list.
        cout << "Any node that has " << subject << " or " << object << " or both in it is removed from the likes list.\n";
        deleteOneLikes(subject, object, headLikes);
    }
    else
    {
        //here we should be able to insert the likes pair, the insert function will handle the position
        cout << line << " relation has been added to the likes list.\n";
        insertOneLikes(subject, object, headLikes);
    }



}


void parseLine(string& sub, string& obj, string& line)
{
    istringstream strStream(line);
    string junk;
    strStream >> sub >> junk >> obj;
}


//Implementations of processing functions
void insertOneLikes(string& sub, string& obj, NodeLike* &headLikes)
{
    /*This function gets the subject/object pair inserts them to the proper
     * position in the likes linked list
     * */

    //create the node
    NodeLike *insert = new NodeLike;
    insert->subject = sub;
    insert->object = obj;
    insert->next = NULL;

    //if list is empty
    if (headLikes == NULL) {
        headLikes = insert;
        return;
    }

    //create navigation pointer
    NodeLike* ptr = headLikes;

    //look for the appropriate position
    while(ptr->next != NULL)
    {
        if (ptr->subject == sub && ptr->next->subject != sub)
        //ensures that element is added at the end of the line of same subject
        {

            //the new node will point to what I'm pointing to
            insert->next = ptr->next;

            //I will point to the node
            ptr->next = insert;
            return;
        }

        //increment
        ptr = ptr->next;
    }

    //if we don't find an element with the same subject, insert to end,
    if (ptr->next == NULL)
    {
        ptr->next = insert;
        return;
    }
}


void insertOneTaken(string& name, NodeTaken* &headTaken)
{
    /*This function takes the name of a person and inserts it to the taken
     * linked list while maintaining alphabetical order
     * */

    //create dynamic node
    NodeTaken * temp= new NodeTaken;
    temp->name = name;
    temp->next = NULL;


    //if list is empty
    if (headTaken == NULL)
    {
        headTaken = temp;
        return;
    }

    //check first element
    if (headTaken->name > temp->name)
    {
        temp->next = headTaken;
        headTaken = temp;
        return;
    }

    //create a navigation pointer
    NodeTaken* ptr = headTaken;


    //find its position and place it
    while(ptr->next != NULL && ptr->next->name < temp->name)
    {
        ptr = ptr->next;
    }

    temp->next = ptr->next;
    ptr->next = temp;
}


void deleteOneLikes(string& sub, string& obj, NodeLike * &headLikes)
{
    /*This function is designed to delete all the elements containing either obj or
     * sub from the likes list
     * */

    bool deleteFirst = false;

    //navigation pointer
    NodeLike* ptr = headLikes;

    //check the first element of the list, or if the list is 1 element long
    if (ptr->subject == sub || ptr->object == sub || ptr->subject == obj || ptr->object == obj)
    {
        if(headLikes->next == NULL)
        //if list 1 element long and it contains the target pair, delete the list
        {
            delete headLikes;
            headLikes = NULL;
        }
        else
        //if first element of the list is what we're looking for
        {
            //take a note of it until we're done looking at the remainder of the list
           deleteFirst = true;
        }
    }


    // TODO: this only goes over the list one time
    while(ptr->next != NULL)
    //look at the next element and check if it is to be deleted
    {
        if (ptr->next->subject == sub || ptr->next->object == sub || ptr->next->subject == obj || ptr->next->object == obj)
            //if sub or obj exist in any parameter of the next node
        {
            //save the next node
            NodeLike* temp = ptr->next;

            //make current node point to the node that temp points to (remove temp from chain)
            ptr->next = temp->next;

            delete temp;
        }

        //increment
        if (ptr->next != NULL && !(ptr->next->subject == sub || ptr->next->object == sub || ptr->next->subject == obj || ptr->next->object == obj)) {
            ptr = ptr->next;
        }
    }

    if (deleteFirst)
    {
        ptr = headLikes;
        headLikes = headLikes->next;
        delete ptr;
    }
}


void deleteLikesAll(NodeLike* headLikes)
{
    /*The function is to delete to whole NodeLike linked list at the end
     * of the program
     * */
    NodeLike *ptr;

    while(headLikes != NULL)
    {
        ptr = headLikes;
        headLikes = headLikes->next;
        delete ptr;

    }
}


void deleteTakenAll(NodeTaken* headTaken)
{
    /* This function is to delete the whole NodeTaken linked list at the end
     * of the program
     * */
    NodeTaken *ptr;

    while(headTaken != NULL) {
        ptr = headTaken;
        headTaken = headTaken->next;
        delete ptr;
    }
}


bool inLikes(const string& sub, const string& obj, NodeLike* headLikes)
{
    /* This function finds if a particular subject/object pair exists in the Likes linked list
     * This function can take reversed parameters object/subject to address 2 test cases
     * */
    while(headLikes != NULL)
    {
        if (headLikes->subject == sub && headLikes->object == obj)
        {
            return true;
        }
        headLikes = headLikes->next;
    }
    return false;
}


bool inTaken(string& name, NodeTaken* headTaken)
{
    /* This function finds a pair in the Taken linked list
     * */
    while(headTaken != NULL)
    {
        if (headTaken->name == name)
        {
            return true;
        }
        headTaken = headTaken->next;
    }
    return false;
}


void printLikes(NodeLike* headLikes)
{
    /*This function prints the likes linked list
     * */
    cout << "Likes list:";

    while(headLikes != NULL) {

        cout << " (" << headLikes->subject << ", " << headLikes->object << ")";
        headLikes = headLikes->next;
    }

    cout << endl;
}


void printTaken(NodeTaken* headTaken)
{
    /*This function prints the taken linked list
     * */
    cout << "Taken list:";

    while(headTaken != NULL) {
        cout << " " << headTaken->name;
        headTaken = headTaken->next;
    }
    cout << endl<< endl;
}