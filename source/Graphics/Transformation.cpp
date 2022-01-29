#include "Transformation.h"

Transformation::Transformation(TransformType type, int32_t starttime, int32_t endtime, int32_t startvalue, int32_t endvalue)
{
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

void Transformation::Update()
{
	int32_t time = GameClock::Clock().Time();
	
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
	
	currentvalue = (int32_t)(((time-starttime)/(float)totaltime)*totalvalue + startvalue);
}

bool Transformation::Active()
{
	if (lastactive)
	{
		lastactive = false;
		return true;
	}
	return active;
}
