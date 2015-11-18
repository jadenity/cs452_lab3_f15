#include <iostream>
#include <algorithm>
#include "Process.hpp"
#include "RTS.hpp"
#include "Time_Queue.hpp"
#include "Scheduler.hpp"

using namespace std;

RTS::RTS(vector<Process *> &processes)
        : Scheduler(processes){
}

RTS::~RTS() {
}

//Note: the 100k processes test file doesn't seem to have any processes with a deadline of 10000 or more

void RTS::run() {
  int clock = 0;
  int jobsLoaded = 0; //keeps track of how many processes have entered the process ecosystem
  int expectedRunTime = 0; //keeps track of burst total of loaded processes, good for kicking out processes that won't finish
  
  //need to create some data structure to hold processes loaded in (priority queue proved too complex)
  //a C++ vector seems to be the simplest to handle
  vector<Process*> rtsQueue; //contains processes sorted by deadline

  //if there's nothing, don't bother trying to populate the vector
  if(this->hasUnfinishedJobs()){
    
    //"while there is a process to load and arrival time is equal to clock,
    //"load them into the rtsQueue and set their state to READY"
    while(jobsLoaded < (int)this->processes.size() && this->processes.at(jobsLoaded)->getArrivalTime() == clock){

      //we should see if processes may be able to complete within the deadline
      rtsQueue.push_back(this->processes.at(jobsLoaded));
      this->processes.at(jobsLoaded)->setState(Process::READY_TO_RUN);
      jobsLoaded++;
    }

    //sort rtsQueue by deadline (this'll be slow, but it's the best idea so far...)
    sort(rtsQueue.begin(), rtsQueue.end(), Process::compareDeadline);

    //TESTING: pop and output some info about processes loaded
    /*for(int i = 0; i < jobsLoaded; i++){
      Process *p = rtsQueue.front();
      rtsQueue.erase(rtsQueue.begin());
      cout << "PID:" << p->getPID() << " Arrival:" << p->getArrivalTime() << " Burst:" << p->getBurst() << " Deadline:" << p->getDeadline() << endl;
    }*/
  }

  //we want to keep going until both data lists say everything is done
  while(this->hasUnfinishedJobs() && rtsQueue.size() > 0){

    //check if the process can't finish by the deadline (while loop dumps all following processes that can't finish)
    while(false){//"if p != null && p->getTimeRemaining + clock > p->getDeadline";
      //TODO: terminate this process
      //load the next process (if there is one)
    }

    //for when there is an active process, CLOCK TICK
    if(false){ //"if p != null (meaning that rtsQueue isn't empty/waiting for a process to arrive)" (false is just a placeholder)
      
      //TODO:perform clock tick on the process
      //p->setTimeRemaining(p->getTimeRemaining - 1);
      
    }

    //advance the clock, last part of CLICK TICK
    clock++;

    //check if the current process is done (ignoring deadline check for now, though it could be turned into a short method)
    if(false){ //"if p != null && p->getTimeRemaining == 0"
      //p->getState() == Process::TERMINATED;
      //p = new Process(rtsQueue->pop); //load next process in rtsQueue
    }

    //check if a process in the main queue has an arival time equal to clock. If there is such process, load it
    //TODO: p2 = a process still in the main queue (one with minimal arrival time)
    if(false){ //"if p2 != null (in which case RTS is done) && p2->arrivalTime <= clock"

      while(false){ //"while p2->arrivalTime <= clock"
	//pop processes off the main queue, add them to rtsQueue, and reorder rtsQueue
	//p = lowest deadline process
      }
      //Order processes by deadline/PID again
    }
  }
}

