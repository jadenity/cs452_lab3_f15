#ifndef WHS_HPP
#define	WHS_HPP

#include <list>
#include "Scheduler.hpp"
#include "RBTree.hpp"

class WHS : public Scheduler {

public:
	WHS(vector<Process *> &processes, int quantum, int ageLimit);
	virtual ~WHS();
    void run();
    void insertIntoTree(int priority, list<Process*> *list);
    void deleteFromTree(int priority);
    void addProcessToPriorityList(Process* p);
    void removeProcessFromPriorityList(Process* p);
    bool receiveNewJobs(int clock);
    void printTree();
private:
	int quantum;
	int ageLimit;
    RBTree rbt;
    rbtree tree;
    deque<Process*> waitQueue;
    // vector<list<Process*>* > listVector;


};

#endif	/* WHS_HPP */