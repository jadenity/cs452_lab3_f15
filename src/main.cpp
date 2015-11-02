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
    if (argc == 2) {
        algorithm = string(argv[1]);
        
        // check if the value entered was 'mfqs', 'rts', or 'hs'
        if (algorithm != "mfqs" && algorithm != "rts" && algorithm != "hs") {
            cout << "Enter 'mfqs', 'rts' or 'hs'" << endl;
            exit(1);
        }
    } else {
        cout << "Enter an algorithm to use. (mfqs, rts, hs)" << endl;
        exit(1);
    }
    
    int numberOfQueues = 0;
    
    // get the number of queues from the user 
    if (algorithm == "mfqs") {
        cout << "Enter how many queues: ";
        cin >> numberOfQueues;
    }

    // get time quantum from the user
    int quantum = 0;
    cout << "Enter the time quantum for the ready-to-run queue: ";
    cin >> quantum;

    // Read in the test file
    ifstream file;
    file.open("test.txt");
    string line;
    vector<string> fields;
    vector<Process*> processes;
    while (file.good()) {
        getline(file, line);
        split(fields, line, is_any_of("\t"));
        Process *p = new Process(atoi(fields.at(0).c_str()),
                                 atoi(fields.at(1).c_str()), 
                                 atoi(fields.at(2).c_str()), 
                                 atoi(fields.at(3).c_str()), 
                                 atoi(fields.at(4).c_str()), 
                                 atoi(fields.at(5).c_str()), 
                                 Process::NEW);
        processes.push_back(p);
    }
    file.close();
    
    // sorts the processes by arrival time
    sort(processes.begin(), processes.end(), Process::compare);
    
    // Create the queues
    if (algorithm == "mfqs") {
        MFQS *mfqs = new MFQS(processes, quantum, numberOfQueues);
        mfqs->run();
    } else if (algorithm == "rts") {
        RTS *rts = new RTS(processes, quantum, numberOfQueues);
        rts->run();
    } 

    return 0;
}

