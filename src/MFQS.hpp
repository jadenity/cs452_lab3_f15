#ifndef MFQS_HPP
#define	MFQS_HPP

#include "Scheduler.hpp"


class MFQS : public Scheduler {
    
public:
    MFQS(vector<Process *> &processes, int quantum, int numberOfQueues, int aging);
    virtual ~MFQS();
    void run();
    bool receiveNewJobs(int clock);
private:
	int aging;

};

#endif	/* MFQS_HPP */

