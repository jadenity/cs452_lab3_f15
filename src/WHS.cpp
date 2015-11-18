#include <iostream>
#include "Process.hpp"
#include "WHS.hpp"
#include "RBTree.hpp"

#define DEBUG

using namespace std;

WHS::WHS(vector<Process *> &processes) 
			: Scheduler(processes) {

}

WHS::~WHS(){
}

void WHS::run() {
	Process* p = new Process(0, 25, 0, 55, 50, 3, Process::NEW);
	RBTree rbt;
	rbtree t = rbt.rbtree_create();
    rbt.print_tree(t);
    for (i = 0; i < 12; i++) {
        int x = rand() % 10;
        int y = rand() % 10;
    	cout<<"Inserting "<< p->getPID() <<" -> "<< p->toString() << endl << endl;
    	rbt.rbtree_insert(t, (void*)(p->getPriority()), (void*)p, Process::compare_priority_tree);
	}

    Process* q = (Process*)(rbt.rbtree_lookup(t, (void*)(p->getPriority()), Process::compare_priority_tree));

    cout << "found: " << q->toString() << endl;
}
