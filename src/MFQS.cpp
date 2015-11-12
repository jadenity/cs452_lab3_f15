#include <iostream>
#include <algorithm>
#include <boost/foreach.hpp>
#include "MFQS.hpp"
#include "Scheduler.hpp"
#include "Time_Queue.hpp"

#define DEBUG

using namespace std;

MFQS::MFQS(vector<Process *> &processes, int quantum, int numberOfQueues, int ageLimit)
        : Scheduler(processes), 
        ageLimit(ageLimit), quantum(quantum), numberOfQueues(numberOfQueues) {
    Time_Queue *readyToRunQ = new Time_Queue(quantum);

    // Add the ready to run queue and then add the other RR,
    // lastly add the FCFS queue to the end.
    this->queues.push_back(readyToRunQ);
    for (int i = 0; i < numberOfQueues - 2; i++) {
        quantum *= 2;
        Time_Queue *currentQ = new Time_Queue(quantum);
        this->queues.push_back(currentQ);
    }
    Time_Queue *fcfs = new Time_Queue();
    this->queues.push_back(fcfs);

    // Add jobs with arrival 0 to first queue
    receiveNewJobs(0);
}

MFQS::~MFQS(){
}

void MFQS::run() {
    int clock = 0;
    while (this->hasUnfinishedJobs()) {
        int i = 0;
        int lastQueue = (int)this->queues.size() - 1;
        bool newProcessesAdded;
        while (i <= lastQueue) {
            Time_Queue *queue = this->queues.at(i);
            bool queueIsEmpty = queue->empty();

#ifdef DEBUG
            if (!queueIsEmpty) {
                cout << "**" << endl;
                cout << "Currently running in queue " << i << ": " << queue->toString();
                cout << "**" << endl << endl;
            }
#endif
            newProcessesAdded = receiveNewJobs(clock);
            while (!queueIsEmpty && !newProcessesAdded) {
                Process *p = queue->pop();

//#ifdef DEBUG
//                cout << "Process " << p->getPID() << " running, time_remaining: " << p->getTimeRemaining() << endl;
//#endif
                int timeRan;
                if (i == lastQueue) { // Last queue is FCFS
                    timeRan = p->getTimeRemaining();

                    // The difference between this clock tick and the end of
                    // its last run session is its added to its wait time.
                    p->addTimeWaiting(clock - p->getexitCPUTick());

                    clock += timeRan;

                    // TODO Add timeRemaining to age of all processes after p that are in the 3rd or lower queue

                    // Terminate the process
                    p->setTimeRemaining(0);
                    p->setFinishTime(clock);
                    p->setState(Process::TERMINATED);

#ifdef DEBUG
                    cout << "****FCFS****" << endl;
                    cout << "Process " << p->getPID() << " ran for " << timeRan << " and terminated." << endl;
                    cout << "clock: " << clock << endl << endl;
#endif
                } else { // All queues before last are TQ
                    p->setState(Process::RUNNING);

                    if (p->getTimeRemaining() <= queue->getQuantum()) { // Process will finish in this TQ
                        timeRan = p->getTimeRemaining();
                        p->addTimeWaiting(clock - p->getexitCPUTick());
                        clock += timeRan;

                        // TODO Add timeRemaining to age of all processes after p that are in the 3rd or lower queue

                        // Terminate the process
                        p->setTimeRemaining(0);
                        p->setFinishTime(clock);
                        p->setState(Process::TERMINATED);

#ifdef DEBUG
                        cout << "Process " << p->getPID() << " ran for " << timeRan << " and terminated." << endl;
                        cout << "clock: " << clock << endl << endl;
#endif
                    } else { // Process will not finish in this TQ
                        timeRan = queue->getQuantum();
                        p->addTimeWaiting(clock - p->getexitCPUTick());
                        clock += timeRan;
                        p->setTimeRemaining(p->getTimeRemaining() - timeRan);

                        // TODO Add quantum to age of all processes after p that are in the 3rd or lower queue

                        // Set the stop clock tick to be the current clock. Next time the process runs,
                        // the difference between the current clock tick and then will be added to its wait time.
                        p->setexitCPUTick(clock);
                        p->setState(Process::READY_TO_RUN);

                        // Demote the process
                        this->queues.at(i + 1)->push(p);

#ifdef DEBUG
                        cout << "Proccess " << p->getPID() << " ran for " << timeRan 
                            << ", time_remaining: " << p->getTimeRemaining()
                            << ", time_waiting: " << p->getTimeWaiting() << endl;
                        cout << "clock: " << clock << endl << endl;
                        cout << "Proccess " << p->getPID() << " added to Queue " << i+1 << ": " << queues.at(i + 1)->toString() << endl;
#endif
                    }
                }

                queueIsEmpty = queue->empty();
                newProcessesAdded = receiveNewJobs(clock);
            } // end while(!queueIsEmpty && !newProcessesAdded)

            // Change i based on why the loop exited
            if (newProcessesAdded) {
                i = 0;
            } else if (queueIsEmpty) { // only check if the queue is empty if there were no new processes
                i++;
            }

        } // end while(i <= lastQueue)

        
#ifdef DEBUG
        cout << "All arrived processes finished. Clock: " << clock << endl << endl;
#endif
        // Increase clock tick so later arrivals get to run 
        clock++;

    } // end while(hasUnfinishedJobs()) 
}

// receiveNewJobs changes the state of all processes that
// have arrived <= clock to READY_TO_RUN, returns whether there
// were any such processes, and adds them to the first queue
bool MFQS::receiveNewJobs(int clock) {
    bool foundNewProcess = false;
    if (clock >= 0) {
        BOOST_FOREACH(Process *p, processes) {
            if ((p->getState() == Process::NEW) &&
                        (p->getArrivalTime() <= clock)) {
                // The process has been waiting since its arrival
                // (exitCPUTick is set to arrival_time upon creation)
                p->addTimeWaiting(clock - p->getexitCPUTick());

                // Reset the exitCPUTick to the current clock,
                // otherwise the time since its arrival will be counted twice.
                p->setexitCPUTick(clock);
                p->setState(Process::READY_TO_RUN);
                // Add the process to the first queue
                this->queues.at(0)->push(p);
                foundNewProcess = true;
#ifdef DEBUG
                std::cout << "New arrival: adding process " << p->getPID() << " to first queue" << endl;
#endif
            }
        }
    }
    return foundNewProcess;
}

bool MFQS::age(int curQ, Process* p, int lastQ, int timeRan) {
    bool jobAged = false;
    int j, startQ;
    // Only add age to queues >= 2 (3rd or lower queues)
    if (curQ < 2) {
        startQ = 2;
    } else {
        startQ = curQ;
    }

    for (int i = startQ; i <= lastQ; i++) {
        Time_Queue* q = queues.at(i);
        for (int j = 0; j < q->size(); j++) {

        }
    }

    return false;
}


// p = process that just finished in CPU
// After a process (p) runs for its time quantum, do this:
// If p finished,
//  Add time remaining to age of all procceses in current queue (q) and all queues beneath q.
//  Set p's time remaining to 0
//  Terminate p.
//  Promote all processes in q and all queues beneath q whose age exceeded the age limit.
//      Only promote processes not in the top two queues.
// else
//  Add time quantum of q to all processes in q and all queues beneath q.
//  Promote all processes in q and all queues beneath q whose age exceeded the age limit.
//      Only promote processes not in the top two queues.
//  Demote p.
