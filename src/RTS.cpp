#include <iostream>
#include <queue>
#include <vector>
#include <functional>
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


//TEMP CODE
template<typename T> void print_queue(T& q) {
    while(!q.empty()) {
        std::cout << q.top() << " ";
        q.pop();
    }
    std::cout << '\n';
}
//END TEMP CODE

void RTS::run() {
  int clock = 0;
  //TODO: IN PROGRESS: make a priority queue of processes sorted by deadline
  //Order processes by deadline/PID (new method?)
  //TODO: Process *p = new Process(rtsQueue->pop);

  //TEMP CODE
  std::priority_queue<int> q;
int n[10] = {1,8,5,6,3,4,0,9,3,2};
  for(int i = 0; i < 10; i++){
      q.push(n[i]);
  }

  print_queue(q);

  std::priority_queue<int, std::vector<int>, std::greater<int> > q2;

  for(int i = 0; i < 10; i++){
      q2.push(n[i]);
  }

  q2.push(5); //adding this shows that values added any time are sorted when pushed into the priority queue
  print_queue(q2);
  //END TEMP CODE

  //TESTING: Do the above for processes, sorting them by deadline
  if(this->hasUnfinishedJobs()){
    int i = 0;
    while(i < (int)this->processes.size()){
      
      i++;
    }
    
  }
  //END TEST CODE

//this->hasUnfinishedJobs()
  while(false){ //may want to check if both the original queue sent in AND rtsQueue have jobs

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

