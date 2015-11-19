#ifndef WHS_HPP
#define	WHS_HPP

#include "Scheduler.hpp"

class WHS : public Scheduler {

public:
	WHS(vector<Process *> &processes, int quantum, int ageLimit);
	virtual ~WHS();
    void run();
private:
	int quantum;
	int ageLimit;


};

#endif	/* WHS_HPP */