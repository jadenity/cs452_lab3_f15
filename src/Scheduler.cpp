#include <boost/foreach.hpp>
#include <sstream>
#include "Scheduler.hpp"
#include "Time_Queue.hpp"
#include <iostream>

Scheduler::Scheduler(vector<Process *> &processes, int quantum, int numberOfQueues) 
                                    : processes(processes) {
    Time_Queue *readyToRunQ = new Time_Queue(quantum);
    BOOST_FOREACH(Process *currentProcess, processes) {
        // Add new incoming jobs to the ready to run queue and set their state
        currentProcess->setState(Process::NEW);
        readyToRunQ->push(currentProcess);
    }

    // Add the ready to run queue and then add the rest
    this->queues.push_back(readyToRunQ);
    for (int i = 0; i < numberOfQueues - 1; i++) {
        quantum *= 2;
        Time_Queue *currentQ = new Time_Queue(quantum);
        this->queues.push_back(currentQ);
    }
}

bool Scheduler::receivedNewProcess(int clock) const {
    bool foundNewProcess = false;
    if (clock >= 0) {
        BOOST_FOREACH(Process *currentProcess, processes) {
            if ((currentProcess->getState() == Process::NEW) &&
                        (currentProcess->getArrivalTime() <= clock)) {
                std::cout << "Making PID " << currentProcess->getPID() << " ready to run" << endl;
                currentProcess->setState(Process::READY_TO_RUN);
                foundNewProcess = true;
            }
        }
    }
    return foundNewProcess;
}

Scheduler::Scheduler(const Scheduler& orig) {
    this->queues = orig.queues;
    this->processes = orig.processes;
}

Scheduler::~Scheduler() {
}

bool Scheduler::hasJobs() const {
    bool hasJobs = false;
    int i = 0;
    
    
    while (!hasJobs && i < (int)this->queues.size()) {
        cout << this->queues.at(i)->toString() << endl;
        if (!this->queues.at(i)->empty()) {
            hasJobs = true;
        }
        i++;
    }
    
    return hasJobs;
}

string Scheduler::toString() const {
    stringstream s;
    BOOST_FOREACH(Time_Queue *currentQ, this->queues) {
        s << currentQ->toString() << endl;
    }
    return s.str();
}

void Scheduler::run(){}