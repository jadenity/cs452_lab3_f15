#ifndef WHS_HPP
#define	WHS_HPP

#include "Scheduler.hpp"

class WHS : public Scheduler {

public:
	WHS(vector<Process *> &processes);
	virtual ~WHS();
    void run();
private:


};

#endif	/* WHS_HPP */