#ifndef MFQS_HPP
#define	MFQS_HPP

#include "Scheduler.hpp"


class MFQS : public Scheduler {
    
public:
    MFQS(vector<Process *> &processes, int quantum, int numberOfQueues, int ageLimit);
    virtual ~MFQS();
    void run();
    bool receiveNewJobs(int clock);
    bool age();
private:
	int ageLimit;

};

#endif	/* MFQS_HPP */

