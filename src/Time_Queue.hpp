#ifndef Time_QUEUE_HPP
#define	Time_QUEUE_HPP

#include <queue>
#include <string>
#include "Process.hpp"

using namespace std;

class Time_Queue {
    
public:
    Time_Queue(int quantum);
    void push(Process  *p);
    Process pop();
    string toString() const;
    bool empty() const;
    int size() const;
    int getQuantum() const;
    deque<Process *> getQueue() const;
    virtual ~Time_Queue();
private:
    int quantum;
    deque<Process *> queue;    
};

#endif	/* MFQ_QUEUE_HPP */

