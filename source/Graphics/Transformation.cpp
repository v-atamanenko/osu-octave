#include "Transformation.h"

Transformation::Transformation(TransformType type, OOTime starttime, OOTime endtime, OOInt startvalue, OOInt endvalue) {
	this->type = type;
	
	this->starttime = starttime;
	this->endtime = endtime;
	
	this->startvalue = startvalue;
	this->endvalue = endvalue;
	
	totaltime = endtime-starttime;
	totalvalue = endvalue-startvalue;
	
	currentvalue = startvalue;
	
	active = false;
	lastactive = false;
}

void Transformation::Update() {
    OOTime time = GameClock::Clock().Time();
	
	if (!active)
	{
		if (time > starttime)
			active = true;
		else
			return;
	}
	
	if (time > endtime)
	{
		active = false;
		currentvalue = endvalue;
		lastactive = true;
		return;
	}
	
	currentvalue = (OOInt)(((OOFloat)(time-starttime)/(OOFloat)totaltime)*(OOFloat)totalvalue + (OOFloat)startvalue);
}

bool Transformation::Active() {
	if (lastactive) {
		lastactive = false;
		return true;
	}
	return active;
}
