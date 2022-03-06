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
	
	if ((InputHelper::KeyDown(SDL_BUTTON_LEFT, IH_KEY_MOUSE) ||
         InputHelper::KeyDown(SDLK_z, IH_KEY_KEYBOARD) ||
         InputHelper::KeyDown(SDLK_x, IH_KEY_KEYBOARD)) && ! InputHelper::BlockKeydown) {
        printf("OnTouchDown %i;\n", hitObject->GetEndTime());
        hitObject->OnTouchDown(touch);
        InputHelper::BlockKeydown = true;
    }
	if ((InputHelper::KeyHeld(SDL_BUTTON_LEFT, IH_KEY_MOUSE) ||
         InputHelper::KeyHeld(SDLK_z, IH_KEY_KEYBOARD) ||
         InputHelper::KeyHeld(SDLK_x, IH_KEY_KEYBOARD)))
		hitObject->OnTouch(touch);
	
	if ((InputHelper::KeyUp(SDL_BUTTON_LEFT, IH_KEY_MOUSE) ||
         InputHelper::KeyUp(SDLK_z, IH_KEY_KEYBOARD) ||
         InputHelper::KeyUp(SDLK_x, IH_KEY_KEYBOARD)))
		hitObject->OnTouchUp(touch);
	
	hitObject->Update();
}
