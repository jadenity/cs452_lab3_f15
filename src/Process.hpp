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
    Process();
    Process(int pid, int burst, int arrival_time, int priority, int deadline, int io, State state);
    Process(const Process &p);
    virtual ~Process();
    string toString() const;
    State getState() const;
    void setState(State s);
    int getArrivalTime() const;
    int getBurst() const;
    int getTimeRemaining() const;
    void setTimeRemaining(int t);
    int getPriority() const;
    int getPID() const;
    int getDeadline() const;
    int getIO() const;
    int getTimeWaiting() const;
    void addTimeWaiting(int time);
    int getStopClockTick() const;
    void setStopClockTick(int clock);
    int getFinishTime() const;
    void setFinishTime(int clock);
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
    int stopClockTick;
    int finishTime;
};

#endif

