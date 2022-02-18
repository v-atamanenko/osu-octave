#include "HitObjectManager.h"

HitObjectManager::~HitObjectManager()
{
	//delete hitobjects
	for (auto & mHitObject : mHitObjects)
	{
		delete mHitObject;
	}
}

void HitObjectManager::Add(HitObject* object)
{
	mHitObjects.push_back(object);
}

void HitObjectManager::HandleInput()
{
	touchPosition touch = InputHelper::TouchRead();
	
	HitObject* hitObject;
	
	// remove all hitobjects before the current time and all hitobjects that have been hit
	bool process = true;
	while (process)
	{
		process = false;
		
		if (mHitObjects.empty())
			return;
		
		// process hitobjects one at a time
		hitObject = mHitObjects.front();
		
		if (hitObject->GetEndTime() <= GameClock::Clock().Time() || hitObject->GetHit())
		{
			// sprites are handled by spritemanager and will not be deleted (see HitObject.h)
			
			// TODO: this is a bit hacky - find a way to guarantee this won't cause a crash
			if (GameClock::Clock().Time() - hitObject->GetEndTime() < 1000 && !hitObject->GetHit())
				hitObject->Hit();
			
			mHitObjects.pop_front();
			delete hitObject;
			
			process = true;
		}
	}
	
	// now we are left with the next hitobject that can react to user interaction
	
	if ((InputHelper::KeyDown(SDL_BUTTON_LEFT, IH_KEY_MOUSE) ||
         InputHelper::KeyDown(SDLK_z, IH_KEY_KEYBOARD) ||
         InputHelper::KeyDown(SDLK_x, IH_KEY_KEYBOARD)))
		hitObject->OnTouchDown(touch);
	
	if ((InputHelper::KeyHeld(SDL_BUTTON_LEFT, IH_KEY_MOUSE) ||
         InputHelper::KeyHeld(SDLK_z, IH_KEY_KEYBOARD) ||
         InputHelper::KeyHeld(SDLK_x, IH_KEY_KEYBOARD)))
		hitObject->OnTouch(touch);
	
	if ((InputHelper::KeyUp(SDL_BUTTON_LEFT, IH_KEY_MOUSE) ||
         InputHelper::KeyUp(SDLK_z, IH_KEY_KEYBOARD) ||
         InputHelper::KeyUp(SDLK_x, IH_KEY_KEYBOARD)))
		hitObject->OnTouchUp(touch);
	
	hitObject->Update();

/*
	//get the first unhit hitobject
	if (mHitObjects.size() == 0)
		return;
	
	HitObject* hitObject = mHitObjects.front();
	
	//only the topmost object can be hit at any time
	if (hitObject->GetEndTime() <= GameClock::Clock().Time() && hitObject->GetHit() == false)
	{
		hitObject->Hit();
	}
	
	if (hitObject->GetHit() == true)
	{
		//sprites are handled by spritemanager and will not be deleted (see HitObject.h)
		delete hitObject;
		mHitObjects.pop_front();
		
		if (mHitObjects.size() == 0)
			return;
		
		hitObject = mHitObjects.front();
	}
	
	//at this point hitObject is pointing to the topmost unhit object if such an object exists
	if (hitObject != NULL)
	{
		if (InputHelper::KeyDown(KEY_TOUCH))
			hitObject->OnTouchDown(touch);
		
		if (InputHelper::KeyHeld(KEY_TOUCH))
			hitObject->OnTouch(touch);
		
		if (InputHelper::KeyUp(KEY_TOUCH))
			hitObject->OnTouchUp(touch);
		
		hitObject->Update();
	}
*/
}