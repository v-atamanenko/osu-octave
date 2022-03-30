#pragma once

#include <cstdio>
#include <list>

#include "HitObjects/HitObject.h"
#include "HitObjects/HitCircle.h"
#include "HitObjects/HitSlider.h"
#include "HitObjects/HitSpinner.h"
#include "Helpers/InputHelper.h"

class HitObjectManager
{
	public:
		void Add(HitObject* object);
		void HandleInput();

        ~HitObjectManager() {
            for (auto & mHitObject : mHitObjects) {
                delete mHitObject;
            }
        };

	protected:
		std::list<HitObject*> mHitObjects;
};
