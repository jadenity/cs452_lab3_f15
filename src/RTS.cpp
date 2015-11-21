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
  int jobsLoaded = 0; //keeps track of how many processes have entered the process ecosystem. points at which process should be loaded next
  int jobsEnded = 0; //counts how many processes got terminated. mostly for testing
  
  //need to create some data structure to hold processes loaded in (priority queue proved too complex)
  //a C++ deque should do. It's like a vectr, but can push_front as well as push_back
  deque<Process*> rtsQueue; //contains processes that have "arrived" sorted by deadline

  //if there's nothing, don't bother trying to populate the deque
  if(this->hasUnfinishedJobs()){
    
    //"while there is a process to load and arrival time is equal to clock,
    //"load them into the rtsQueue and set their state to READY"
    while(jobsLoaded < (int)this->processes.size() && this->processes.at(jobsLoaded)->getArrivalTime() == clock){

      //we should see if processes may be able to complete within the deadline
      //I'm wondering if we should try to predict if a process won't finish due to so many other processes being loaded...
      if(this->processes.at(jobsLoaded)->getDeadline() > this->processes.at(jobsLoaded)->getBurst()){ //burst < deadline
        rtsQueue.push_back(this->processes.at(jobsLoaded)); //process is added to rtsQueue, making it READY
        this->processes.at(jobsLoaded)->setState(Process::READY_TO_RUN);
        jobsLoaded++;
      } else { //process won' be able to finish, so terminate it
	    this->processes.at(jobsLoaded)->setState(Process::TERMINATED);
	    jobsLoaded++;
	    jobsEnded++;
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
  while(this->hasUnfinishedJobs()){

    //check if we have processes that can't finish by the deadline
    while(p->getTimeRemaining() + clock > p->getDeadline() /*|| p->getTimeRemaining() == 0*/){ //shouldn't need 2nd argument
      p->setState(Process::TERMINATED); //since p is a pointer, it is set as terminated in the scheduler's list as well
      jobsEnded++;
cout << jobsEnded << " processes ended!" << endl;

      //load the next process, if there is one in rtsQueue
      if(rtsQueue.size() > 0){
        p = rtsQueue.front();
		rtsQueue.pop_front();
      } else {
	    p = NULL;
      }
	  for (int i = 0; i < (int)rtsQueue.size(); i++) { //TESTING: display PIDs of laoded processes
        cout << rtsQueue.at(i)->getPID() << endl;
      }
    }

    //CLOCK TICK, reduce time remaining on active process
    if(p != NULL){
      p->setTimeRemaining(p->getTimeRemaining() - 1);
    }

    //advance the clock, last part of CLICK TICK (occurs with or without an active process)
    //clock tick related output also belongs here
    clock++;

    //check if the current process is done (if one is active)--------------
    if(p != NULL && p->getTimeRemaining() == 0){

      p->setState(Process::TERMINATED);
      jobsEnded++;
cout << p->getPID() << " ended." << endl;

      //pop next process from rtsQueue, if there is one
      if(rtsQueue.size() > 0){
        p = rtsQueue.front();
		rtsQueue.pop_front();
      } else {

	    p = NULL;
      }
	  for (int i = 0; i < (int)rtsQueue.size(); i++) { //TESTING: display PIDs of processes loaded into rtsQueue
        cout << rtsQueue.at(i)->getPID() << endl;
      }
    }

    //check if a process has "arrived" (unless we loaded them all)
    if(jobsLoaded < (int)this->processes.size() && this->processes.at(jobsLoaded)->getArrivalTime() == clock){

      //push the current active process back into rtsQueue perhaps this was wrong? this was causing multiples of a single process being in rtsQueue
      //rtsQueue.push_front(p);
	  
	  for (int i = 0; i < (int)rtsQueue.size(); i++) { //TESTING: display PIDs of processes loaded into rtsQueue
        cout << rtsQueue.at(i)->getPID() << endl;
      }

      while(jobsLoaded < (int)this->processes.size() && this->processes.at(jobsLoaded)->getArrivalTime() == clock){

	if(this->processes.at(jobsLoaded)->getDeadline() > this->processes.at(jobsLoaded)->getBurst() + clock){

          rtsQueue.push_back(this->processes.at(jobsLoaded));
          this->processes.at(jobsLoaded)->setState(Process::READY_TO_RUN);
          jobsLoaded++;
	  cout << "JOBS:" << jobsLoaded << endl;

        } else {
          this->processes.at(jobsLoaded)->setState(Process::TERMINATED); //this "at jobsLoaded" here should be fine
          jobsLoaded++;
	  cout << "JOBS:" << jobsLoaded << endl;
        }

      //Order processes by deadline again
      sort(rtsQueue.begin(), rtsQueue.end(), Process::compareDeadline);
      }
    }
  }
}
