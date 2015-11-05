#include <iostream>
#include <algorithm>
#include <boost/foreach.hpp>
#include "MFQS.hpp"
#include "Scheduler.hpp"
#include "Time_Queue.hpp"

#define DEBUG

using namespace std;

MFQS::MFQS(vector<Process *> &processes, int quantum, int numberOfQueues, int aging)
        : Scheduler(processes, quantum, numberOfQueues), 
        aging(aging){
}

MFQS::~MFQS(){
}

void MFQS::run() {
    int clock = 0;
    while (this->hasJobs()) {
        int i = 0;
        int lastQueue = (int)this->queues.size() - 1;
        while (i <= lastQueue) {
            Time_Queue *queue = this->queues.at(i);
#ifdef DEBUG
            cout << "**" << endl;
            cout << "Starting queue " << i << ": " << queue->toString();
            cout << "**" << endl << endl;
#endif

            // receivedNewProcess changes the state of all processes that
            // have arrived <= clock to READY_TO_RUN and returns whether there
            // were any such processes
            while (!queue->empty() && !receivedNewProcess(clock)) {
                Process *p =  new Process(queue->pop());

                if (p->getState() == Process::READY_TO_RUN) {
#ifdef DEBUG
                    cout << "Process " << p->getPID() << " running, time_remaining: " << p->getTimeRemaining() << endl;
#endif
                    if (i == lastQueue) { // Last queue is FCFS
                        // Terminate process, advance clock remaining burst
                        p->setState(Process::TERMINATED);
                        clock += p->getTimeRemaining();
#ifdef DEBUG
                        cout << "**FCFS**" << endl;
                        cout << "Process " << p->getPID() << " ran for " << p->getTimeRemaining() << " and terminated." << endl;
                        cout << "clock " << clock << endl << endl;
#endif
                        p->setTimeRemaining(0);
                    } else {
                        p->setState(Process::RUNNING);

                        if (p->getTimeRemaining() <= queue->getQuantum()) { // Process will finish in this TQ
                            p->setState(Process::TERMINATED);
                            clock += p->getTimeRemaining();
#ifdef DEBUG
                            cout << "Process " << p->getPID() << " ran for " << p->getTimeRemaining() << " and terminated." << endl;
                            cout << "clock " << clock << endl << endl;
#endif
                            p->setTimeRemaining(0);
                        } else { // Process will not finish in this TQ
                            p->setState(Process::READY_TO_RUN);
                            clock += queue->getQuantum();
                            p->setTimeRemaining(p->getTimeRemaining() - queue->getQuantum());
                            this->queues.at(i + 1)->push(p);
#ifdef DEBUG
                            cout << "Proccess " << p->getPID() << " ran for " << queue->getQuantum() 
                                            << ", time_remaining: " << p->getTimeRemaining() << endl;
                            cout << "clock " << clock << endl << endl;
                            cout << "Proccess " << p->getPID() << " added to Queue " << i+1 << ": " << queues.at(i + 1)->toString() << endl;
#endif
                        }
                    }
                }
            }
            // Change i based on why the loop exited
            if (queue->empty()) {
                i++;
            } else if (receivedNewProcess(clock)) {
                i = 0;
            } // else don't change i, continue on current queue

        }
        // Increase clock tick so later arrivals get their state switched
        clock++;
        cout << clock << endl;
    }
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
