//
// Created by nuh alsharafi on 3/22/2024.
//

#include "EmployeeProject2DLL.h"


EmployeeProject2DLL :: EmployeeProject2DLL(void)
{
   head_2DHLL = nullptr;
}


bool EmployeeProject2DLL :: isEmployeeAssignedToProject(const string& e_name, const string& p_name)
{
    /* This function checks if an employee-project pair exists or not
     * */
    EmployeeNode* traverse_2DHLL = head_2DHLL;

    //look for employee
    while(traverse_2DHLL != nullptr)
    {
        if(traverse_2DHLL->employee_name == e_name)
        //found the employee with the name
        {
            ProjectNode* traverse_projects = traverse_2DHLL->head;
            //look for project under employee name
            while(traverse_projects != nullptr)
            {
                if (traverse_projects->project_name == p_name)
                //found the project under the employee
                {
                    return true;
                }

                traverse_projects = traverse_projects->next;
            }
        }
        traverse_2DHLL = traverse_2DHLL->down;
    }
    //no employee, or project under the employee was found
    return false;
}


bool EmployeeProject2DLL :: updateProjectPriority(const string& e_name, const string& p_name, int& priority)
{
    /* The function return false if there's another project with the same priority for the employee
     * The function return false if the project's priority is the same
     * The function updates the priority and reorders the list if needed
     * it is assumed that the employee-project pair exists
     * */

    //find the employee
    EmployeeNode* traverse_2DHLL = head_2DHLL;
    while(traverse_2DHLL != nullptr)
    {
        if(traverse_2DHLL->employee_name == e_name)
        //found the employee, now check the conditions
        {
            //check if another project exists with the same priority
            ProjectNode* traverse_projects = traverse_2DHLL->head;
            while(traverse_projects != nullptr)
            {
                if(traverse_projects->project_priority == priority && traverse_projects->project_name != p_name)
                //found a project with the same priority and a different name
                {
                    cout << "The project priority has not been updated because there is another project with the same priority.\n";
                    return false;
                }
                //increment
                traverse_projects = traverse_projects->next;
            }


            //reset traverse & find the project
            traverse_projects = traverse_2DHLL->head;
            while(traverse_projects != nullptr)
            {
                if(traverse_projects->project_name == p_name)
                //found the project
                {
                    //check if the project's priority is equal to the new one
                    if(traverse_projects->project_priority == priority)
                    {
                        cout << "The project priority is already the same as the new priority.\n";
                        return false;
                    }
                    else
                    {
                        //update project priority
                        int old_priority = traverse_projects->project_priority;
                        traverse_projects->project_priority = priority;
                        priority = old_priority;
                        //sort
                        sortProjects(traverse_projects);
                        return true;
                    }

                }
                //increment
                traverse_projects= traverse_projects->next;
            }
        }
        //increment the employee loop
        traverse_2DHLL = traverse_2DHLL->down;
    }

    return false; //just in case
}


bool EmployeeProject2DLL :: assignEmployeeToProject(const string& e_name, const string& p_name, int priority)
{
    /* Function returns false if the project cannot be added to an employee because there is another project with
     * identical priority, returns true if assignment done successfully
     * */

    EmployeeNode* employee_loc = nullptr;
    if(employeeExists(e_name, employee_loc))
    //employee exists, and we have his location
    {
        //check employee projects for containing priority conflict
        ProjectNode* traverse_projects = employee_loc->head;
        while(traverse_projects != nullptr)
        {
            if(traverse_projects->project_priority == priority)
            //found a priority conflict
            {
                cout << "The project has not been added because there is another project with the same priority.\n";
                return false;
            }

            //increment
            traverse_projects = traverse_projects->next;
        }
    }
    else
    //did not find employee, create new one, and keep his address
    {
        EmployeeNode* new_employee = new EmployeeNode(e_name, nullptr, nullptr, nullptr);
        insertEmployeeSorted(new_employee);
        employee_loc = new_employee; //save employee's address
    }

    //assign project, employee should be assigned to employee_loc
    if(employee_loc != nullptr)
    //this is for defensive purposes
    {
        ProjectNode* new_project = new ProjectNode(p_name, priority, nullptr, nullptr);

        if(employee_loc->head == nullptr && employee_loc->tail == nullptr)
        //if employee has no assigned project because he's new
        {
            employee_loc->head = new_project;
            employee_loc->tail = new_project;
            return true; //exit the function
        }

        //add to tail
        employee_loc->tail->next = new_project;
        new_project->prev = employee_loc->tail;
        employee_loc->tail = new_project;
        //sort to find its correct position
        sortProjects(new_project);
        return true;
    }

    //we should not have to use this.
    return false;

}


void EmployeeProject2DLL :: withdrawEmployeeFromProject(const string& e_name, const string& p_name, int& priority)
{
    /* This function deletes an employee-project pair and returns its priority via priority parameter
     * It also has to handle deleting the employee if he has no projects left.
     * */

    ProjectNode* project_loc = nullptr;
    EmployeeNode* employee_loc = nullptr;

    //find employee project pair
    EmployeeNode* traverse_2DHLL = head_2DHLL;
    while(traverse_2DHLL != nullptr)
    {
        if(traverse_2DHLL->employee_name == e_name)
        //found the employee
        {
            employee_loc = traverse_2DHLL; //save employee
            //look for project
            ProjectNode* traverse_projects = traverse_2DHLL->head;
            while(traverse_projects != nullptr)
            {
                if(traverse_projects->project_name == p_name)
                //found the project
                {
                    project_loc = traverse_projects; //save project
                    priority = project_loc->project_priority; // return priority
                }
                //increment
                traverse_projects = traverse_projects->next;
            }
        }
        //increment
        traverse_2DHLL = traverse_2DHLL->down;
    }

    //delete
    if(employee_loc != nullptr && project_loc != nullptr)
    //defensive programming
    {
        //check head and tail
        if(employee_loc->head == project_loc)
        {
            employee_loc->head = project_loc->next;

        }
        if(employee_loc->tail == project_loc)
        {
            employee_loc->tail = project_loc->prev;
        }

        //re-route pointers
        if(project_loc->prev != nullptr)
        //check if there's a node before project to re-route
        {
            project_loc->prev->next = project_loc->next; //the next of previous should be my next
        }
        if(project_loc->next != nullptr)
        //check if there's a node after project to re-route
        {
            project_loc->next->prev = project_loc->prev; //the previous of next should be my previous
        }

        //project_loc is isolated from the doubly-linked list, safe to delete
        delete project_loc;

        //check if employee has projects assigned and delete him
        employeeHasProjects(employee_loc);
    }
}


void EmployeeProject2DLL :: printTheEntireList()
{
    /* Function prints the entire 2DHLL list
     * */
    if(head_2DHLL == nullptr)
    //check if list is empty
    {
        cout << "The list is empty.\n";
    }
    else
    {
        //employees are stored in ascending order, go through them
        EmployeeNode* traverse_2DHLL = head_2DHLL;
        while(traverse_2DHLL != nullptr)
        {
            cout << traverse_2DHLL->employee_name << ":";

            //traverse said employee's projects by ascending priority
            ProjectNode* traverse_pr = traverse_2DHLL->head;
            while(traverse_pr != nullptr)
            {
                cout << " (" << traverse_pr->project_name << ", " << traverse_pr->project_priority << ")";

                //increment
                traverse_pr = traverse_pr->next;
            }
            cout << endl;

            //increment
            traverse_2DHLL = traverse_2DHLL->down;
        }
    }
}


void EmployeeProject2DLL :: printEmployeeProjects(const string& e_name, int order)
{
    /* Function prints an employee's projects according to a predetermined format
     * */

    if(head_2DHLL == nullptr)
    //check if employee list is empty
    {
        cout << "There are no employees in the list.\n";
        return;
    }

    //find the employee
    EmployeeNode* traverse_2DHLL = head_2DHLL;
    while(traverse_2DHLL != nullptr && traverse_2DHLL->employee_name != e_name)
    {
        //increment
        traverse_2DHLL = traverse_2DHLL->down;
    }

    //iterate over employee projects and print them
    if (traverse_2DHLL != nullptr)
    //defensive programming bit
    {
        if(order == 1)
            //ascending order
        {
            ProjectNode* traverse_pr = traverse_2DHLL->head;
            while(traverse_pr->next != nullptr)
            {
                cout << "(" << traverse_pr->project_name << ", " << traverse_pr->project_priority << ") ";
                //increment
                traverse_pr = traverse_pr ->next;
            }
            //print the last element
            cout << "(" << traverse_pr->project_name << ", " << traverse_pr->project_priority << ")" << endl;
            //this time no space in the end
            return;
        }
        else
            //descending order
        {
            ProjectNode* traverse_pr = traverse_2DHLL->tail;
            while(traverse_pr->prev != nullptr)
            {
                cout << "(" << traverse_pr->project_name << ", " << traverse_pr->project_priority << ") ";
                //increment
                traverse_pr = traverse_pr->prev;
            }
            //print last element without space
            cout << "(" << traverse_pr->project_name << ", " << traverse_pr->project_priority << ")" << endl;
            return;
        }
    }
    else
    //could not find the employee
    {
        cout << "The employee is not in the list.\n";
    }
}


void EmployeeProject2DLL :: undo(char operation, const string& e_name, const string& p_name, int priority)
{
    /* Function checks the type of operation that was done (indicated by operation variable) then
     * carries out the appropriate actions to reverse the operation
     * */

    if(operation == 'a')
    //a project was 'a'ssigned
    {
        //this means that the operation that was done was assigning a project, so you un-assign it
        withdrawEmployeeFromProject(e_name, p_name, priority);
        cout << "Undoing the assignment of a project.\n";
    }
    else if(operation == 'w')
    //a project was 'w'ithdrawn
    {
        assignEmployeeToProject(e_name, p_name, priority);
        cout << "Undoing the withdrawal of a project.\n";
    }
    else if(operation == 'u')
    //a priority was 'u'pdated
    {
        updateProjectPriority(e_name, p_name, priority);
        cout << "Undoing the update of a project priority.\n";
    }
}


void EmployeeProject2DLL :: clear()
{
    /* This function clears all the dynamically allocated memory
     * */
    while(head_2DHLL != nullptr)
    //loop over all employees
    {
        //delete all projects
        ProjectNode* traverse_projects = head_2DHLL->tail;
        ProjectNode* temp;
        while(traverse_projects != nullptr)
        //loop over all project
        {
            temp = traverse_projects;
            traverse_projects = traverse_projects->prev;
            delete temp;
        }

        //move to next employee and delete current one
        EmployeeNode* temp_employee = head_2DHLL;
        head_2DHLL = head_2DHLL->down;
        delete temp_employee;
    }
}


//Private function implementations

void EmployeeProject2DLL ::sortProjects(ProjectNode *current)
{
    /* This function sorts the projects of an employee using a pointer to one of the projects
     * The function expects the pointer to point to a misplaced item, but still checks the whole
     * list regardless.
     * Be careful, this function assumes there every project has a unique priority, i.e. no duplicate priorities
     * */

    //find the correct place for the object I pointed to
    bool sorted = false;
    while(!sorted)
    {
        //check if we can and should move the node right
        if(current->next != nullptr && current->project_priority > current->next->project_priority)
        //current and current->next should be swapped
        {
            swapProjects(current, current->next);
        }

        //check if we can and should move the node to the left
        if(current->prev != nullptr && current->project_priority < current->prev->project_priority)
        {
            swapProjects(current, current->prev);
        }

        //check if the whole list is sorted
        //go to right edge
        while(current->next != nullptr)
        {
            current = current->next;
        }

        //go back to the left edge and check that the values are in the current order
        while(current->prev != nullptr && current->project_priority > current->prev->project_priority)
        //the loop will break once we get to the end or hit a pair of elements that are not sorted
        {
            current = current->prev;
        }

        //check which condition stopped the loop
        if(current->prev == nullptr)
        //we reached the end of the list and they're all sorted
        {
            sorted = true;
        }
    }

    //the workings of this function are questionable, I'll probably be back here when I debug
}


void EmployeeProject2DLL :: swapProjects(ProjectNode* swapFrom, ProjectNode* swapTo)
{
    /* This function swaps 2 projects on the project doubly linked list
     * this function avoids re-routing all the pointers by just swapping the values of
     * the project nodes.
     * */

    //create a copy of swapFrom values
    string temp_p_name = swapFrom->project_name;
    int temp_priority = swapFrom->project_priority;

    //assign swapTo values to swapFrom
    swapFrom->project_priority = swapTo->project_priority;
    swapFrom->project_name = swapTo->project_name;

    //assign the backup values to swapTo
    swapTo->project_priority = temp_priority;
    swapTo->project_name = temp_p_name;
}


bool EmployeeProject2DLL :: employeeExists(const string& e_name, EmployeeNode* & location)
{
    /* Function tests employee_name linked list to see if an employee name is there.
     * if function finds employee, returns true along with pointer to location of employee.
     * */

    EmployeeNode* traverse_2DHLL = head_2DHLL;
    while(traverse_2DHLL != nullptr)
    {
        if(traverse_2DHLL->employee_name == e_name)
        {
            location = traverse_2DHLL;
            return true;
        }

        //increment
        traverse_2DHLL = traverse_2DHLL->down;
    }
    return false;
}


void EmployeeProject2DLL :: employeeHasProjects(EmployeeNode* employee)
{
    /* Function tests whether the given employee has projects assigned to him
     * Deletes him if there's no projects assigned
     * */

    if(employee->head == nullptr || employee->tail == nullptr)
    /* both && and || work, but || is safer since it will delete the employee if there's
    a mistake in the pointers related to his doubly linked list */
        //employee has no projects assigned
    {
        //check if it's the first node
        if(head_2DHLL->employee_name == employee->employee_name)
        {
            EmployeeNode* temp = head_2DHLL;
            head_2DHLL = head_2DHLL->down;
            delete temp;
            return;
        }

       //find the node before
        EmployeeNode* traverse_2DHLL = head_2DHLL;
       while(traverse_2DHLL->down != nullptr && traverse_2DHLL->down->employee_name != employee->employee_name)
       //this loop will stop one node before the one to be deleted
       {
           //increment
           traverse_2DHLL = traverse_2DHLL->down;
       }

       //delete employee
       EmployeeNode* temp = traverse_2DHLL->down;
       traverse_2DHLL->down = temp->down; //move node out of sequence
       delete temp;
       return;
    }
}


void EmployeeProject2DLL :: insertEmployeeSorted(EmployeeNode *employee)
{
    /* Function inserts new employee node into employee list while maintaining alphabetical order
     * */

    EmployeeNode* traverse_2DLL = head_2DHLL;
    //check if head_2DHLL is empty
    if(head_2DHLL == nullptr)
    {
        head_2DHLL = employee;
        return;
    }

    //check if new employee should go first
    if(traverse_2DLL->employee_name > employee->employee_name)
    {
        employee->down = traverse_2DLL;
        head_2DHLL = employee;
        return;
    }

    while(traverse_2DLL->down != nullptr)
    //find the employee's proper place
    {
        if(traverse_2DLL->down->employee_name > employee->employee_name)
        //insert Employee
        {
            employee->down = traverse_2DLL->down;
            traverse_2DLL->down = employee;
            return;
        }

        //increment
        traverse_2DLL = traverse_2DLL->down;
    }

    //If by this point the function hasn't exited, it means the employee is to be added to the end of the list
    employee->down = nullptr;
    traverse_2DLL->down = employee;
}