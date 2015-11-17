#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <algorithm>
#include "Process.hpp"
#include "Scheduler.hpp"
#include "MFQS.hpp"
#include "RTS.hpp"

#define DEBUG

using namespace std;
using namespace boost;

int main(int argc, char** argv) {
    
    string algorithm = "";
    
    // makes sure the user entered just one argument
//    if (argc == 2) {
//        algorithm = string(argv[1]);
//        
//        // check if the value entered was 'mfqs', 'rts', or 'hs'
//        if (algorithm != "mfqs" && algorithm != "rts" && algorithm != "hs") {
//            cout << "Enter 'mfqs', 'rts' or 'hs'" << endl;
//            exit(1);
//        }
//    } else {
//        cout << "Enter an algorithm to use. (mfqs, rts, hs)" << endl;
//        exit(1);
//    }
    
    cout << "Which algorithm would you like to use? (mfqs/rts/whs): ";
    cin >> algorithm;
    while (algorithm != "mfqs" && algorithm != "rts" && algorithm != "whs") {
        cout << "Which algorithm would you like to use? (mfqs/rts/whs): ";
        cin >> algorithm;
    }

    
    int numberOfQueues = 0;
    int aging = 0;
    int quantum = 0;
    
    if (algorithm == "mfqs") {
        // get the number of queues from the user 
        cout << "Enter how many queues: ";
        cin >> numberOfQueues;

        // get the aging wait time
        cout << "Enter wait time for aging: ";
        cin >> aging;

        // get time quantum from the user
        quantum = 0;
        cout << "Enter the time quantum for the ready-to-run queue: ";
        cin >> quantum;
    }


    // Read in the test file
    ifstream file;
    file.open("10proc.txt");
    string line;
    vector<string> fields;
    vector<Process*> processes;
    getline(file, line);
    // The test file MUST have a newline at the end in order for this to work.
    while (file.good()) {
        split(fields, line, is_any_of("\t"));
        Process *p = new Process(atoi(fields.at(0).c_str()),
                                 atoi(fields.at(1).c_str()), 
                                 atoi(fields.at(2).c_str()), 
                                 atoi(fields.at(3).c_str()), 
                                 atoi(fields.at(4).c_str()), 
                                 atoi(fields.at(5).c_str()), 
                                 Process::NEW);
        processes.push_back(p);
        getline(file, line);
    }
    file.close();
    
    // sorts the processes by arrival time
    sort(processes.begin(), processes.end(), Process::compare);
    
    // Create the queues
    Scheduler* sched;
    if (algorithm == "mfqs") {
        sched = new MFQS(processes, quantum, numberOfQueues, aging);
    } else if (algorithm == "rts") {
        sched = new RTS(processes);
    } 
    
    // Using polymorphism to call functions after "new"ing schedulers.
    sched->run();
    double avgWaitTime = sched->calcAvgWaitTime();
    cout << "Average wait time: " << avgWaitTime << endl << endl;

    double avgTurnaroundTime = sched->calcAvgTurnaroundTime();
    cout << "Average turnaround time: " << avgTurnaroundTime << endl << endl;

    return 0;
}

