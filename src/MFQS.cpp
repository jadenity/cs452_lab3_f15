#include <iostream>
#include <algorithm>
#include <boost/foreach.hpp>
#include "MFQS.hpp"
#include "Scheduler.hpp"
#include "Time_Queue.hpp"

#define DEBUG

using namespace std;

MFQS::MFQS(vector<Process *> &processes, int quantum, int numberOfQueues) 
        : Scheduler(processes, quantum, numberOfQueues){    
}

MFQS::~MFQS(){
}

void MFQS::run() {
    int clock = 0;
    while (this->hasJobs()) {
#ifdef DEBUG
                cout << "queues.size() = " << this->queues.size() << endl;
#endif
        for (int i = 0; i <= (int)this->queues.size() - 1; i++) {
            Time_Queue *queue = this->queues.at(i);
#ifdef DEBUG
            cout << "**" << endl;
            cout << "Starting queue " << i << ": " << queue->toString();
            cout << "**" << endl << endl;
#endif
            while (!queue->empty()) {
                Process *p =  new Process(queue->pop());

                if (p->getState() == Process::READY_TO_RUN) {
#ifdef DEBUG
                cout << "Process " << p->getPID() << " running, time_remaining: " << p->getTimeRemaining() << endl;
#endif
                    if (i == ((int)this->queues.size() - 1)) { // Last queue is FCFS
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

                        if (p->getTimeRemaining() <= queue->getQuantum()) {
                            p->setState(Process::TERMINATED);
                            clock += p->getTimeRemaining();
#ifdef DEBUG
                            cout << "Process " << p->getPID() << " ran for " << p->getTimeRemaining() << " and terminated." << endl;
                            cout << "clock " << clock << endl << endl;
#endif
                            p->setTimeRemaining(0);
                        } else {
                            p->setState(Process::READY_TO_RUN);
                            clock += queue->getQuantum();
                            p->setTimeRemaining(p->getTimeRemaining() - queue->getQuantum());
#ifdef DEBUG
                            cout << "Proccess " << p->getPID() << " ran for " << queue->getQuantum() 
                                            << ", time_remaining: " << p->getTimeRemaining() << endl;
                            cout << "clock " << clock << endl << endl;
#endif
                            this->queues.at(i + 1)->push(p);
#ifdef DEBUG
                            cout << "Proccess " << p->getPID() << " added to Queue " << i+1 << ": " << queues.at(i + 1)->toString() << endl;
#endif
                        }
                    }
                }
            }
        }

        Time_Queue *last = this->queues.at(this->queues.size() - 1);
        if (!last->empty()) {
            BOOST_FOREACH (Process *p, last->getQueue()) {
                clock += p->getTimeRemaining();
                p->setState(Process::TERMINATED);
                p->setTimeRemaining(0);
            } 
        }
    }
}