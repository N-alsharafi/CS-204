//
// Created by nuh alsharafi on 3/22/2024.
//

#ifndef HOMEWORK3_EMPLOYEEPROJECT2DLL_H
#define HOMEWORK3_EMPLOYEEPROJECT2DLL_H

#include <string>
#include <iostream>
using namespace std;


struct ProjectNode
{
    string project_name;
    int project_priority;
    ProjectNode * next;
    ProjectNode * prev;

    ProjectNode()
    {}

    ProjectNode(const string& p_name, int p_priority, ProjectNode* n, ProjectNode* p)
    : project_name(p_name), project_priority(p_priority), next(n), prev(p)
    {}
};


struct EmployeeNode
{
    string employee_name;
    ProjectNode * head;
    ProjectNode * tail;
    EmployeeNode * down;

    EmployeeNode()
    {}

    EmployeeNode(const string& e_name, ProjectNode * h, ProjectNode* t, EmployeeNode* d)
    : employee_name(e_name), head(h), tail(t), down(d)
    {}
};



class EmployeeProject2DLL {
private:
    EmployeeNode * head_2DHLL;
    void sortProjects(ProjectNode* current);
    void swapProjects(ProjectNode* swapFrom, ProjectNode* swapTo);
    bool employeeExists(const string& e_name, EmployeeNode* & location);
    void employeeHasProjects(EmployeeNode* employee);
    void insertEmployeeSorted(EmployeeNode* employee);
    // TODO add the helper private functions
public:
    EmployeeProject2DLL(void);
    bool isEmployeeAssignedToProject(const string& e_name, const string& p_name);
    bool updateProjectPriority(const string& e_name, const string& p_name, int& priority);
    bool assignEmployeeToProject(const string& e_name, const string& p_name, int priority);
    void withdrawEmployeeFromProject(const string& e_name, const string& p_name, int& priority);
    void printTheEntireList();
    void printEmployeeProjects(const string& e_name, int order);
    void undo(char operation, const string& e_name, const string& p_name, int priority);
    void clear();

};


#endif //HOMEWORK3_EMPLOYEEPROJECT2DLL_H
