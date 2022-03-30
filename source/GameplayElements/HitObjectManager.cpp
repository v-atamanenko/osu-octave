#include "HitObjectManager.h"

void HitObjectManager::Add(HitObject* object) {
	mHitObjects.push_back(object);
}

void HitObjectManager::HandleInput() {
	touchPosition touch = InputHelper::TouchRead();
	
	HitObject* hitObject;
	
	// remove all hitobjects before the current time and all hitobjects that have been hit
	bool process = true;
	while (process) {
		process = false;
		
		if (mHitObjects.empty())
			return;
		
		// process hitobjects one at a time
		hitObject = mHitObjects.front();
		
		if (hitObject->GetEndTime() <= GameClock::Clock().Time() || hitObject->GetHit()) {
			// Sprites are handled by spritemanager and will not be deleted (see HitObject.h)
			if ((GameClock::Clock().Time() - hitObject->GetEndTime() < 1000) && ! hitObject->GetHit()) {
                hitObject->Hit();
            }

			mHitObjects.pop_front();
			delete hitObject;

			process = true;
		}
	}
	
	// now we are left with the next hitobject that can react to user interaction
	
	if (InputHelper::KeyDown(Control::IH_CONTROL_ACTION) && ! InputHelper::BlockKeydown) {
        hitObject->OnTouchDown(touch);
        InputHelper::BlockKeydown = true;
    }

	if (InputHelper::KeyHeld(Control::IH_CONTROL_ACTION)) {
        hitObject->OnTouch(touch);
    }

	if (InputHelper::KeyUp(Control::IH_CONTROL_ACTION)) {
        hitObject->OnTouchUp(touch);
    }

	hitObject->Update();
}
