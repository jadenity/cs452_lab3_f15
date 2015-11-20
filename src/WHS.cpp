#include <iostream>
#include "Process.hpp"
#include "WHS.hpp"
#include "RBTree.hpp"

#define DEBUG

using namespace std;

WHS::WHS(vector<Process *> &processes, int quantum, int ageLimit) 
			: Scheduler(processes),
            quantum(quantum),
            ageLimit(ageLimit) {
}

WHS::~WHS(){
}

void WHS::run() {
    Process* p;
    RBTree rbt;
    rbtree t = rbt.rbtree_create();

    rbt.print_tree(t);

    p = new Process(0, 25, 0, 55, 50, 3, Process::NEW);
    cout << "Inserting process " << p->getPID() <<" with priority " << p->getPriority() << " -> "<< p->toString() << endl << endl;
    rbt.rbtree_insert(t, (void*)p, (void*)p, Process::compare_priority_tree);

    rbt.print_tree(t);

    for (int i = 0; i < 10; i++) {
        int randPriority = rand() % 99;
        p = new Process(i+1, 25, 0, randPriority, 50, 3, Process::NEW);
    	cout << "Inserting process " << p->getPID() <<" with priority " << p->getPriority() << " -> "<< p->toString() << endl << endl;
    	rbt.rbtree_insert(t, (void*)p, (void*)p, Process::compare_priority_tree);

        rbt.print_tree(t);
	}

    p = new Process(11, 25, 0, 55, 50, 3, Process::NEW);
    cout << "Inserting process " << p->getPID() <<" with priority " << p->getPriority() << " -> "<< p->toString() << endl << endl;
    rbt.rbtree_insert(t, (void*)p, (void*)p, Process::compare_priority_tree);
    p->setTimeRemaining(4);

    rbt.print_tree(t);

    Process* lookup = new Process(11,0,0,55,0,0,Process::NEW);
    
    Process* q;
    if ( (q = (Process*)(rbt.rbtree_lookup(t, (void*)lookup, Process::compare_priority_tree))) == NULL) {
        cout << "No process with priority " << lookup->getPriority() << " and PID " << lookup->getPID() << " found." << endl;
    } else {
        cout << "found: " << q->toString() << endl;
    }
    delete lookup;

    Process* lookup2 = new Process(11,0,0,55,0,0,Process::NEW);
    
    Process* q2;
    if ( (q2 = (Process*)(rbt.rbtree_lookup(t, (void*)lookup2, Process::compare_priority_tree))) == NULL) {
        cout << "No process with priority " << lookup2->getPriority() << " and PID " << lookup2->getPID() << " found." << endl;
    } else {
        cout << "found: " << q2->toString() << endl;
    }
    delete q2;

}
