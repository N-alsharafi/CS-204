//
// Created by nuh alsharafi on 3/22/2024.
//

#include "UndoStack.h"


//Constructor
UndoStack :: UndoStack(void)
{
    /* Creates an empty stack
     * */
    top = nullptr;
}


void UndoStack :: push(char op, const string& e_name, const string& p_name, int priority)
{
    /* Function inspired by Lab-5 DynamicStringStack */
    StackNode* node = new StackNode(op, e_name, p_name, priority, top);
    top = node;
}


void UndoStack :: pop(char& op, string& e_name, string& p_name, int& priority)
{
    /* Function inspired by Lab-5 DynamicStringStacks */

    if (isEmpty())
    {
        return;
    }
    else
    {
        op = top->operation;
        e_name = top->employee_name;
        p_name = top->project_name;
        priority = top->project_priority;

        //remove top
        StackNode* temp;
        temp = top;
        top = top->next;
        delete temp;
    }
}


bool UndoStack :: isEmpty()
{
    if (top == nullptr){
        return true;
    }
    return false;
}


void UndoStack :: clear()
{
    /* Function inspired by Lab-5 DoublyLinkedList */
    StackNode* temp;
    while(top != nullptr)
    {
        temp = top;
        top = top->next;
        delete temp;
    }
}