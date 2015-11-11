#include <boost/foreach.hpp>
#include <sstream>
#include "Scheduler.hpp"
#include "Time_Queue.hpp"
#include <iostream>

Scheduler::Scheduler(vector<Process *> &processes, int quantum, int numberOfQueues) 
    : processes(processes){
}

//bool Scheduler::receivedNewProcess(int clock) {
//    bool foundNewProcess = false;
//    if (clock >= 0) {
//        BOOST_FOREACH(Process *currentProcess, processes) {
//            if ((currentProcess->getState() == Process::NEW) &&
//                        (currentProcess->getArrivalTime() <= clock)) {
//                std::cout << "Making PID " << currentProcess->getPID() << " ready to run" << endl;
//                currentProcess->setState(Process::READY_TO_RUN);
//                foundNewProcess = true;
//            }
//        }
//    }
//    return foundNewProcess;
//}

Scheduler::Scheduler(const Scheduler& orig) {
    this->queues = orig.queues;
}

Scheduler::~Scheduler() {
}

// Needs to check not if queues are empty but if there are processes on the list
bool Scheduler::hasUnfinishedJobs() const {
//    bool hasJobs = false;
//    int i = 0;
//    
//    while (!hasJobs && i < (int)this->queues.size()) {
//        cout << this->queues.at(i)->toString() << endl;
//        if (!this->queues.at(i)->empty()) {
//            hasJobs = true;
//        }
//        i++;
//    }
//    
//    return hasJobs;
    bool hasUnfinishedJobs = false;
    int i = 0;

    while (!hasUnfinishedJobs && i < (int)this->processes.size()) {
        Process* p = this->processes.at(i);
        if (p->getState() != Process::TERMINATED) {
            cout << "NOT TERMINATED: " << p->toString() << endl;
            hasUnfinishedJobs = true;
        }
        i++;
    }

    return hasUnfinishedJobs;
}

string Scheduler::toString() const {
    stringstream s;
    BOOST_FOREACH(Time_Queue *currentQ, this->queues) {
        s << currentQ->toString() << endl;
    }
    return s.str();
}

void Scheduler::run(){}

