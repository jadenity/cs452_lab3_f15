#include <iostream>
#include <stdlib.h>
#include <list>
#include <algorithm>
#include "Process.hpp"
#include "WHS.hpp"
#include "RBTree.hpp"

#define DEBUG

using namespace std;



WHS::WHS(vector<Process *> &processes, int quantum, int ageLimit) 
			: Scheduler(processes),
            quantum(quantum),
            ageLimit(ageLimit)
             {

    // Make a tree to hold lists of processes at every priority
    tree = rbt.rbtree_create();

    // Make a vector to hold all the lists (even if empty)
    // vector<list<Process*>* > tempVector(100, new list<Process*>());
    vector<list<Process*>* > tempVector;

    for (int i = 0; i <= 99; i++) {
        tempVector.push_back(new list<Process*>());
    }

    // Assign it to the member variable
    listVector = tempVector;

    // Add jobs with arrival 0 to first queue
    receiveNewJobs(0);

}

WHS::~WHS(){
}

void WHS::run() {

    int clock = 0;
    // while (this->hasUnfinishedJobs()) {


        // Choose the highest priority process that is ready to run.
        list<Process*> *maxList = getMaxPriorityList();

        // Run the first process in the list and pop it off the list.
        Process* p = maxList->front();
        maxList->pop_front();

        int runTimer;
        int io = p->getIO();
        int burst = p->getBurst();
        bool killAfterTQ = false;
        if (io == 0) { // CPU bound (no I/O): run for full TQ
            if (burst < quantum) { // Process terminates
                runTimer = burst;
                killAfterTQ = true;
            } else {
                runTimer = quantum;
            }
#ifdef DEBUG
            cout << "Process " << p->getPID() << " is CPU bound (no I/O). Running for " << runTimer << endl;
#endif

            // Save total time in CPU
            int timeInCPU = runTimer;

            // The difference between this clock tick and the end of
            // its last run session is its added to its wait time.
            p->addTimeWaiting(clock - p->getExitCPUTick());

            while (runTimer > 0) {
                p->decrementTimeRemaining();

                // Age processes

                // Check I/O wait queue


                runTimer--;
                clock++;
            }

            if (killAfterTQ) {
                p->setFinishTime(clock);
                p->setState(Process::TERMINATED);
            } else {
                int priorityBefore = p->getPriority();

                // Decrement priority by amount of clock ticks spent in CPU
                p->decrementPriority(timeInCPU);

                // Move the process to the new priority queue if it changed
                if (p->getPriority() < priorityBefore) {
                    removeProcessFromPriorityList(p, priorityBefore);
                    addProcessToPriorityList(p);
                }

                // For wait time calculation, set exit CPU clock tick
                p->setExitCPUTick(clock);
            }
        } else { // Process does I/O (at second-to-last tick of TQ)

        }

    // }










    // node max = rbt.maximum_node(tree->root);
    // while (max != NULL) {
    //     rbt.rbtree_delete(tree, (void*)max->key, RBTree::compare_int);
    //     max = rbt.maximum_node(tree->root);
    //     cout << "ok" << endl;
    // }

    // Delete all the priority lists
    for (int i = 0; i < (int)listVector.size(); i++) {
        delete listVector[i];
    }

    delete tree;
}

list<Process*>* WHS::getMaxPriorityList() {
    return (list<Process*>*)rbt.maximum_node(tree->root)->value;
}

void WHS::insertIntoTree(int priority, list<Process*> *list) {
    rbt.rbtree_insert(tree, (void*)priority, (void*)list, RBTree::compare_int);
}

int WHS::deleteFromTree(int priority) {
    return rbt.rbtree_delete(tree, (void*)priority, RBTree::compare_int);
}

void WHS::addProcessToPriorityList(Process* p) {
    int priority = p->getPriority();
    list<Process*> *priorityList = listVector.at(priority);

    // If the list is empty, add the list to the tree
    if (priorityList->empty()) {
        insertIntoTree(priority, priorityList);
#ifdef DEBUG
        cout << "List " << priority << " was empty. Added to tree:" << endl;
        printTree();
#endif
    } else {
#ifdef DEBUG
        cout << "List " << priority << " was not empty. Not added to tree." << endl;
        printTree();
#endif

    }


    priorityList->push_back(p);
#ifdef DEBUG
    cout << "Added " << p->getPID() << " to list " << priority << endl;
    cout << "List " << priority << ": " << endl;

    for (list<Process*>::iterator it = priorityList->begin(); it != priorityList->end(); ++it) {
        cout << "   " << (*it)->toString() << endl;
    }
#endif
}

void WHS::removeProcessFromPriorityList(Process* p, int priority) {
    list<Process*> *priorityList = listVector.at(priority);

    if (!priorityList->empty()) {
        bool procFound = false;
        Process* curP;
        list<Process*>::iterator it = priorityList->begin();


        while (!procFound && (it != priorityList->end())) {
            curP = *it;
            if (curP->getPID() == p->getPID()) { // Found process
                procFound = true;
                priorityList->erase(it);
#ifdef DEBUG
                cout << "Process " << curP->getPID() << " removed from list " << curP->getPriority() << endl;
#endif
                if (priorityList->empty()) {
                    deleteFromTree(priority);
#ifdef DEBUG
                    cout << "List " << priority << " now empty. Deleted from tree: " << endl;
                    printTree();
#endif
                }
            }
            ++it;
        }
    } else {
        cout << "list " << priority << " empty" << endl;
    }

}

bool WHS::receiveNewJobs(int clock) {
    bool foundNewProcess = false;
    int i = 0;
    bool foundLaterProcess = false;
    Process* p;
    // Since processes are in order of their arrival, stop loop
    // when a later process is found.
    while (i < (int)processes.size() && !foundLaterProcess) {
        p = processes.at(i);
        if ((p->getState() == Process::NEW) && (p->getArrivalTime() <= clock)) { // new, earlier process
            // For debugging, lets us see the time waiting earlier
            p->addTimeWaiting(clock - p->getExitCPUTick());
            
            // Reset the exitCPUTick to the current clock,
            // otherwise the time since its arrival will be counted twice.
            p->setExitCPUTick(clock);

            p->setState(Process::READY_TO_RUN);
            // Add the process to its appropriate priority list
            addProcessToPriorityList(p);
            foundNewProcess = true;
#ifdef DEBUG
            cout << "New arrival at clock " << clock << ": added process " << p->getPID() << " to list " << p->getPriority() << endl;
#endif
        } else if (p->getArrivalTime() > clock) { // later process
            foundLaterProcess = true;
        }
        i++;
    }
    return foundNewProcess;
}

void WHS::printTree() {
    rbt.print_tree(tree);
}
