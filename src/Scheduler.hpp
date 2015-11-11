#ifndef Scheduler_HPP
#define	Scheduler_HPP

#include <vector>
#include <string>
#include "Process.hpp"
#include "Time_Queue.hpp"

class Scheduler {
    
public:
    Scheduler(vector<Process *> &processes, int quantum, int numberOfQueues);
    Scheduler(const Scheduler& orig);
    string toString() const;
    //bool receivedNewProcess(int clock) const;
    bool hasUnfinishedJobs() const;
    virtual void run();
    virtual ~Scheduler();
protected:
    vector<Time_Queue *> queues;
    vector<Process *> processes;
};

#endif

