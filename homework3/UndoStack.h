//
// Created by nuh alsharafi on 3/22/2024.
//

#ifndef HOMEWORK3_UNDOSTACK_H
#define HOMEWORK3_UNDOSTACK_H

#include <string>
#include <iostream>
using namespace std;


struct StackNode
{
    char operation;
    string employee_name;
    string project_name;
    int project_priority;
    StackNode * next;

    StackNode()
    {}

    StackNode(char op, const string& e_name, const string& p_name, int priority, StackNode * n)
    : operation(op), employee_name(e_name), project_name(p_name), project_priority(priority), next(n)
    {}

};

class UndoStack {
private:
    StackNode* top;
public:
    UndoStack(void);
    void push(char op, const string& e_name, const string& p_name, int priority);
    void pop(char& op, string& e_name, string& p_name, int& priority);
    bool isEmpty();
    void clear();
};


#endif //HOMEWORK3_UNDOSTACK_H
