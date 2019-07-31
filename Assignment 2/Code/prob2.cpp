/** @file prob2.cpp
 *  @brief Problem Statement 2 :  Demonstrate both Zombie and Orphan process.
 *
 *  @author Ayush Agarwal, 17114017
 *  @date July 2019
    */
#include <bits/stdc++.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

using namespace std;

int main()
{

    cout << "Parent pid: " << getpid() << endl
         << endl;

    pid_t child_pid = fork(); // created child from parent

    if (child_pid > 0)
    { // parent is executing
        cout << "Parent is active..." << endl;
        sleep(4);
        cout << "Parent is terminated after 2 seconds" << endl;
    }

    else if (child_pid == 0) // child is executing
    {
        cout << "Child is created with pid " << getpid() << " from parent having pid " << getppid() << endl;
        child_pid = fork(); // grandchild is created
        if (child_pid > 0) // child is executing
        {
            sleep(1);
            cout << "Child is sleeping..." << endl;
            sleep(2);
            cout << "Child awake again and active!" << endl;
            sleep(2);
            cout << "Child is now orphan!" << endl
                 << endl;
        }
        else if (child_pid == 0)
        {
            cout << "Grandchild is created with pid " << getpid() << " from parent pid " << getppid() << endl
                 << endl;
            sleep(1);
            cout << "Terminating the grandchild" << endl;
            cout << "Grandchild is now zombie" << endl
                 << endl;
        }
    }

    return 0;
}