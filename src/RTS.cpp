#include "RTS.hpp"
#include "Scheduler.hpp"

using namespace std;

RTS::RTS(vector<Process *> &processes, int quantum, int numberOfQueues)
        : Scheduler(processes, quantum, numberOfQueues){
}

RTS::~RTS() {
}

void RTS::run() {
    
}

