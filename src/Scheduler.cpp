#include <boost/foreach.hpp>
#include <sstream>
#include "Scheduler.hpp"
#include "Time_Queue.hpp"

Scheduler::Scheduler(vector<Process *> &processes, int quantum, int numberOfQueues) {
    
    Time_Queue *readyToRunQ = new Time_Queue(quantum);
    BOOST_FOREACH(Process *currentProcess, processes) {
        // Add new incoming jobs to the ready to run queue and set their state
        currentProcess->setState(Process::READY_TO_RUN);
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

Scheduler::Scheduler(const Scheduler& orig) {
    this->queues = orig.queues;
}

Scheduler::~Scheduler() {
}

bool Scheduler::hasJobs() const {
    bool hasJobs = false;
    int i = 0;
    
    
    while (!hasJobs && i < this->queues.at(i)->size()) {
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