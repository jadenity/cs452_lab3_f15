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

//Note: This was first edited in VIM, then edited in Notepad++ later

void RTS::run() {
  int clock = 0;
  int jobsLoaded = 0; //keeps track of how many processes have entered the process ecosystem. points at which process should be loaded next
  bool hardRealTimeStop = false; //true if process didn't finish using hard real time mode
  char softOrHard; //
  
  //need to create some data structure to hold processes loaded in (making priority queue compatible proved too complex)
  //a C++ deque should do.
  deque<Process*> rtsQueue; //contains processes that have "arrived" sorted by deadline

  //if there's nothing, don't bother trying to populate the deque
  if(this->hasUnfinishedJobs()){
  
    //prompt user to choose soft or hard real time
    cout << "Chose soft or hard real time mode (s/h): ";
    cin >> softOrHard;
	cout << endl;
    
    //"while there is a process to load and arrival time is equal to clock,
    //"load them into the rtsQueue and set their state to READY"
    while(jobsLoaded < (int)this->processes.size() && this->processes.at(jobsLoaded)->getArrivalTime() == clock){

      //we should see if processes may be able to complete within the deadline
      //I'm wondering if we should try to predict if a process won't finish due to so many other processes being loaded...
      if(this->processes.at(jobsLoaded)->getDeadline() > this->processes.at(jobsLoaded)->getBurst()){ //burst < deadline
        rtsQueue.push_back(this->processes.at(jobsLoaded)); //process is added to rtsQueue, making it READY
        this->processes.at(jobsLoaded)->setState(Process::READY_TO_RUN);
        jobsLoaded++;
		
      /* This is currently redundant since the main function handles processes with negative or very low deadlines
	    } else { //process won't be able to finish, so terminate it
	    this->processes.at(jobsLoaded)->setState(Process::TERMINATED);
	    jobsLoaded++;*/
      }
    }

    //sort rtsQueue by deadline (this'll be slow since we have to do this often, but it's the best idea so far...)
    sort(rtsQueue.begin(), rtsQueue.end(), Process::compareDeadline);

    //TESTING: pop and output some info about processes loaded
    /*for(int i = 0; i < jobsLoaded; i++){
      Process *p = rtsQueue.front();
      rtsQueue.erase(rtsQueue.begin());
      cout << "PID:" << p->getPID() << " Arrival:" << p->getArrivalTime() << " Burst:" << p->getBurst() << " Deadline:" << p->getDeadline() << endl;
    }*/
  }

  //in preparation to run RTS, pop the first process off of rtsQueue (earliest PID with shortest deadline).
  //
  //there's also "slack" to keep in mind, the amount of time a process could wait before it must run.
  //In other words, we should probably load the lowest burst processes, even though we shouldn't know burst in a real program.
  //But how should we do this? also sorting by burst may be tricky, and also slow...
  //I think I'll ignore this "slack" thing for now
  Process *p = rtsQueue.front();
  rtsQueue.pop_front();
  //We're done when all processes are terminated either from having too short a deadline or finishing its burst
  while(this->hasUnfinishedJobs() && !hardRealTimeStop){

    //check if we have processes that can't finish by the deadline
	if(softOrHard == 'h'){ //hard real time mode is active. stop scheduler loop now
	  hardRealTimeStop = true;
	  cout << "Process " << p->getPID() << " cancelled. Scheduler halted (hard real time mode). Clock: " << clock << " Time Remaining: " << p->getTimeRemaining() 
				<< " Deadline: " << p->getDeadline() << endl; //output info of cancelled process
	} else {
		while(p->getTimeRemaining() + clock > p->getDeadline()){
		  p->setState(Process::TERMINATED); //terminates current active process. since p is a pointer, it is set as terminated in the scheduler's list as well
		  
		  cout << "Process " << p->getPID() << " cancelled. Clock: " << clock << " Time Remaining: " << p->getTimeRemaining() 
				<< " Deadline: " << p->getDeadline() << endl; //output info of cancelled process
		  
		  //load the next process, if there is one in rtsQueue
		  if(rtsQueue.size() > 0){
			p = rtsQueue.front();
			rtsQueue.pop_front();
		  } else {
			p = NULL;
		  }
		  
#ifdef DEBUG
		  for (int i = 0; i < (int)rtsQueue.size(); i++) { //TESTING: display PIDs of processes loaded into rtsQueue
			cout << rtsQueue.at(i)->getPID() << endl;
		  }
#endif
		  
		}
	

		//CLOCK TICK, reduce time remaining on active process
		if(p != NULL){
		  p->setTimeRemaining(p->getTimeRemaining() - 1);
		}
		
		//increment wait times of all processes loaded in thus far
		for(int i = 0; i < (int)rtsQueue.size(); i++){
		  Process* p2 = rtsQueue.at(i);
		  p2->addTimeWaiting(1);
		}

		//advance the clock, last part of CLICK TICK (occurs with or without an active process)
		clock++;

		//check if the current process is done (if one is active)
		if(p != NULL && p->getTimeRemaining() == 0){

		  p->setFinishTime(clock);
		  p->setState(Process::TERMINATED);
#ifdef DEBUG		  
		  //cout << p->getPID() << " ended." << endl; //TESTING: output PID of process that just ended
#endif
		  //pop next process from rtsQueue, if there is one
		  if(rtsQueue.size() > 0){
			p = rtsQueue.front();
			rtsQueue.pop_front();
		  } else {

			p = NULL;
		  }
		  
		  
#ifdef DEBUG
		  for (int i = 0; i < (int)rtsQueue.size(); i++) { //TESTING: display PIDs of processes loaded into rtsQueue
			cout << rtsQueue.at(i)->getPID() << endl;
		  }
#endif
		  
		  
		}

	  //check if a process has "arrived" (unless we loaded them all)
	  if(jobsLoaded < (int)this->processes.size() && this->processes.at(jobsLoaded)->getArrivalTime() == clock){

		  //push the current active process back into rtsQueue. 
		  //this was wrong somehow, as this was causing multiples of a single process to be in rtsQueue
		  //rtsQueue.push_front(p);
#ifdef DEBUG
		  for (int i = 0; i < (int)rtsQueue.size(); i++) { //TESTING: display PIDs of processes loaded into rtsQueue
			cout << rtsQueue.at(i)->getPID() << endl;
		  }
#endif
		while(jobsLoaded < (int)this->processes.size() && this->processes.at(jobsLoaded)->getArrivalTime() == clock){

			if(this->processes.at(jobsLoaded)->getDeadline() > this->processes.at(jobsLoaded)->getBurst() + clock){ //process can finish by deadline

			  rtsQueue.push_back(this->processes.at(jobsLoaded));
			  this->processes.at(jobsLoaded)->setState(Process::READY_TO_RUN);
			  jobsLoaded++;

			} else { //process can't finish by deadline
		      cout << "Process " << p->getPID() << " cancelled. Clock: " << clock << " Time Remaining: " << p->getTimeRemaining() 
				<< " Deadline: " << p->getDeadline() << endl; //output info of cancelled process
				
			  this->processes.at(jobsLoaded)->setState(Process::TERMINATED);
			  jobsLoaded++;
			}

		  //Order processes by deadline again
		  sort(rtsQueue.begin(), rtsQueue.end(), Process::compareDeadline);
	    }
      }
    }
  }
}
