#include "Time_Queue.hpp"
#include "Scheduler.hpp"
#include "Process.hpp"
#include <boost/foreach.hpp>
#include <sstream>

using namespace std;

Time_Queue::Time_Queue(int quantum) 
        : quantum(quantum),
          queue(deque<Process *>()){
}

Time_Queue::~Time_Queue() {
}

void Time_Queue::push(Process *p) {
    this->queue.push_back(p);
}

Process Time_Queue::pop() {
    Process p = *this->queue.front();
    this->queue.pop_front();
    return p;
}

Process Time_Queue::front() {
    return *this->queue.front();
}

string Time_Queue::toString() const {
    stringstream s;
    s << "Time Quantum = " << this->quantum << endl;
    s << "Queue contents: " << endl;
    if (!this->empty()) {
        BOOST_FOREACH (Process *p, this->queue) {
            s << p->toString() << endl;
        }
    } else {
        s << "Queue is empty" << endl;
    }
    return s.str();
}

bool Time_Queue::hasReadyProcess() const {
    BOOST_FOREACH (Process *p, this->queue) {
        if (p->getState() == Process::READY_TO_RUN) {
            return true;
        }
    }
    return false;
}

bool Time_Queue::empty() const {
    return this->queue.empty();
}

int Time_Queue::size() const {
    return this->queue.size();
}

deque<Process *> Time_Queue::getQueue() const {
    return this->queue;
}

int Time_Queue::getQuantum() const {
    return this->quantum;
}