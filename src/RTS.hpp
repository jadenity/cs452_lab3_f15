#ifndef RTS_HPP
#define	RTS_HPP

#include "Scheduler.hpp"

class RTS : public Scheduler {
    
public:
    RTS(vector<Process *> &processes, int quantum, int numberOfQueues);
    void run();
    virtual ~RTS();
private:

};

#endif	/* RTS_HPP */

