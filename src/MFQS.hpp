#ifndef MFQS_HPP
#define	MFQS_HPP

#include "Scheduler.hpp"


class MFQS : public Scheduler {
    
public:
    MFQS(vector<Process *> &processes, int quantum, int numberOfQueues);
    virtual ~MFQS();
    void run();
protected:
    void round_robin(int clock);

};

#endif	/* MFQS_HPP */

