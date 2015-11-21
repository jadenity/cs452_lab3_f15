#include <iostream>
#include <stdlib.h>
#include <list>
#include <algorithm>
#include "Process.hpp"
#include "WHS.hpp"
#include "RBTree.hpp"

#define DEBUG

using namespace std;

// Make a vector to hold all the lists (even if empty)
vector<list<Process*>* > listVector(100, new list<Process*>());

WHS::WHS(vector<Process *> &processes, int quantum, int ageLimit) 
			: Scheduler(processes),
            quantum(quantum),
            ageLimit(ageLimit)
             {

    // Make a tree to hold lists of processes at every priority
    tree = rbt.rbtree_create();

    // listVector.resize(100);
    // for (int i = 0; i <= 99; i++) {
    //     listVector.push_back(new list<Process*>);
    // }

    // Add jobs with arrival 0 to first queue
    receiveNewJobs(0);

}

WHS::~WHS(){
}

void WHS::run() {

    int clock = 0;
    // while (this->hasUnfinishedJobs()) {
    cout << "processes.at(0): " << processes.at(0)->toString() << endl;
    // removeProcessFromPriorityList(processes.at(0));
    // list<Process*> list = listVector.at(2);
    // if (list.empty()) {
    //     cout << "empty from out here" << endl;
    // }
    //     clock++;
    // }


}

void WHS::insertIntoTree(int priority, list<Process*> *list) {
    rbt.rbtree_insert(tree, (void*)priority, (void*)&list, RBTree::compare_int);
}

void WHS::deleteFromTree(int priority) {
    rbt.rbtree_delete(tree, (void*)priority, RBTree::compare_int);
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
    }

    priorityList->push_back(p);
}

void WHS::removeProcessFromPriorityList(Process* p) {
    int priority = p->getPriority();
    list<Process*> *priorityList = listVector.at(priority);

    if (!priorityList->empty()) {
        cout << "not empty" << endl;
        bool procFound = false;
        Process* curP;
        list<Process*>::iterator it = priorityList->begin();

        // If the begin and end iterators are the same,
        // there is only one process in the list.
        if (priorityList->begin() == priorityList->end()) {
            deleteFromTree(priority);
#ifdef DEBUG
            cout << "List " << priority << " now empty. Deleted from tree: " << endl;
            printTree();
#endif 
        } else { // Otherwise, there is more than one process in the list.
            while (!procFound && (it != priorityList->end())) {
                curP = *it;
                if (curP->getPID() == p->getPID()) { // Found process
                    procFound = true;
                    priorityList->erase(it);
                }
                ++it;
            }
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
            cout << "New arrival: added process " << p->getPID() << " to list " << p->getPriority() << endl;
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
