#include <nds.h>
#include <stdio.h>
#include <list>

#include "HitObjects/HitObject.h"
#include "HitObjects/HitCircle.h"
#include "HitObjects/HitSlider.h"
#include "HitObjects/HitSpinner.h"
#include "Helpers/InputHelper.h"

#ifndef __HITOBJECTMANAGER_H__
#define __HITOBJECTMANAGER_H__



typedef std::list<HitObject*>::iterator hitObjectIterator;

class HitObjectManager
{
	public:
		~HitObjectManager();
		
		void Add(HitObject* object);
		void HandleInput();
	
	protected:
		std::list<HitObject*> mHitObjects;
};

#endif

