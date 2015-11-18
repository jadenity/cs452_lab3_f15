#include <sstream>
#include "Process.hpp"

using namespace std;

Process::Process(){} 

Process::Process(int pid, int burst, int arrival_time, int priority, int deadline, int io, State state) 
        : pid(pid),
          burst(burst),
          arrival_time(arrival_time),
          priority(priority),
          deadline(deadline),
          io(io),
          state(state),
          time_remaining(burst),
          time_waiting(0),
          age(0),
          // Set the stop tick to the arrival time for future calculation of wait time
          exitCPUTick(arrival_time){
}

// Copy Constructor
Process::Process(const Process &p) {
  this->pid = p.getPID();
  this->burst = p.getBurst();
  this->arrival_time = p.getArrivalTime();
  this->priority = p.getPriority();
  this->deadline = p.getDeadline();
  this->io = p.getIO();
  this->state = p.getState();
  this->time_remaining = p.getTimeRemaining();
  this->time_waiting = p.getTimeWaiting();
}

Process::~Process() {
}

string Process::toString() const {
    stringstream s;
    s << "pid: " << this->pid << ", burst: " << this->burst << ", arrival_time: " << this->arrival_time << ", priority: " << this->priority << ", deadline: " << this->deadline << ", io: " << this->io << ", time_remaining: " << this->time_remaining << ", time_waiting: " << this->time_waiting;
    return s.str();
}

void Process::setState(State s) {
    this->state = s;
}

int Process::getArrivalTime() const {
    return this->arrival_time;
}

bool Process::compare(Process *p1, Process *p2) {
    if (p1->getArrivalTime() == p2->getArrivalTime()) {
        return p1->priority < p2->priority;
    } else {
        return p1->getArrivalTime() < p2->getArrivalTime();   
    }
}

// Compares priorities for use with RBTree
int Process::compare_priority_tree(void* leftp, void* rightp) {
    int left = (intptr_t)leftp;
    int right = (intptr_t)rightp;
    if (left > right) { // Higher priority = higher in tree
        return -1;
    } else if (left < right) {
        return 1;
    } else {
        return 0;
    }
}

Process::State Process::getState() const {
    return this->state;
}

int Process::getBurst() const {
    return this->burst;
}

void Process::setTimeRemaining(int t) {
    this->time_remaining = t;
}

int Process::getTimeRemaining() const {
    return this->time_remaining;
}

int Process::getTimeWaiting() const {
    return this->time_waiting;
}

void Process::addTimeWaiting(int time) {
    this->time_waiting += time;
}

int Process::getPriority() const {
    return this->priority;
}

int Process::getPID() const {
    return this->pid;
}

int Process::getDeadline() const {
    return this->deadline;
}

int Process::getIO() const {
    return this->io;
}

int Process::getexitCPUTick() const {
    return this->exitCPUTick;
}

void Process::setexitCPUTick(int clock) {
    this->exitCPUTick = clock;
}

int Process::getFinishTime() const {
    return this->finishTime;
}

void Process::setFinishTime(int clock) {
    this->finishTime = clock;
}

int Process::getAge() const {
    return this->age;
}

void Process::addAge(int age) { 
    this->age += age;
}

void Process::setAge(int age) {
    this->age = age;
}
