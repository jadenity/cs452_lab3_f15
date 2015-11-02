#ifndef PROCESS_HPP
#define	PROCESS_HPP
#include <string>

using namespace std;

class Process {
    
public:
    enum State {
    	NEW,
    	READY_TO_RUN,
    	WAITING_FOR_IO,
    	RUNNING,
    	TERMINATED
    };
    Process(int pid, int burst, int arrival_time, int priority, int deadline, int io, State state);
    virtual ~Process();
    string toString() const;
    void setState(State s);
    int getArrivalTime() const;
    State getState() const;
    int getBurst() const;
    void setTimeRemaining(int t);
    int getTimeRemaining() const;
    int getPriority() const;
    int getPID() const;
    static bool compare(Process *p1, Process *p2);

private:
    int pid;
    int burst;
    int arrival_time;
    int priority;
    int deadline;
    int io;
    State state;
    int time_remaining;
    int time_waiting;
};

#endif
