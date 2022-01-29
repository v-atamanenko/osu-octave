#include "InputHelper.h"
#include <algorithm>

std::vector<SDL_Event> InputHelper::sdlEvents;
std::vector<int> InputHelper::heldControllerKeys;
std::vector<int> InputHelper::heldKeyboardKeys;
std::vector<int> InputHelper::heldMouseButtons;
touchPosition InputHelper::mTouch;

int InputHelper::PollSDL()
{
    SDL_Event event;
    SDL_MouseButtonEvent mbe;
    SDL_MouseMotionEvent mme;

    sdlEvents.clear();
    while ( SDL_PollEvent(&event) ) {
        sdlEvents.push_back(event);

        switch (event.type) {
            case SDL_QUIT:
                return -1;
            case SDL_CONTROLLERBUTTONDOWN:
                heldControllerKeys.push_back(event.cbutton.button);
                break;
            case SDL_CONTROLLERBUTTONUP:
                heldControllerKeys.erase(std::remove(heldControllerKeys.begin(), heldControllerKeys.end(), event.cbutton.button), heldControllerKeys.end());
                break;
            case SDL_KEYDOWN:
                heldKeyboardKeys.push_back(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                heldKeyboardKeys.erase(std::remove(heldKeyboardKeys.begin(), heldKeyboardKeys.end(), event.key.keysym.sym), heldKeyboardKeys.end());
                break;
            case SDL_MOUSEBUTTONDOWN:
                mbe = (SDL_MouseButtonEvent) event.button;
                mTouch.px = mbe.x;
                mTouch.py = mbe.y;

                heldMouseButtons.push_back(mbe.button);
                break;
            case SDL_MOUSEBUTTONUP:
                mbe = (SDL_MouseButtonEvent) event.button;
                mTouch.px = mbe.x;
                mTouch.py = mbe.y;

                heldMouseButtons.erase(std::remove(heldMouseButtons.begin(), heldMouseButtons.end(), mbe.button), heldMouseButtons.end());
                break;
            case SDL_MOUSEMOTION:
                mme = (SDL_MouseMotionEvent) event.motion;
                mTouch.px = mme.x;
                mTouch.py = mme.y;
                break;
        }
    }

    return 0;
}

bool InputHelper::KeyDown(int key, int type)
{
    for (SDL_Event e : sdlEvents) {
        if ( e.type == SDL_CONTROLLERBUTTONDOWN &&
             e.cbutton.button == (SDL_GameControllerButton) key &&
             type == IH_KEY_CONTROLLER ) {
            return true;
        }
        if ( e.type == SDL_KEYDOWN &&
             e.key.keysym.sym == (SDL_KeyCode) key &&
             type == IH_KEY_KEYBOARD ) {
            return true;
        }
        if ( e.type == SDL_MOUSEBUTTONDOWN && type == IH_KEY_MOUSE ) {
            SDL_MouseButtonEvent mbe = (SDL_MouseButtonEvent) e.button;
            if (mbe.button == key) {
                return true;
            }
        }
    }
	return false;
}

bool InputHelper::KeyHeld(int key, int type)
{
    switch (type) {
        case IH_KEY_MOUSE:
            if (std::find(heldMouseButtons.begin(), heldMouseButtons.end(), key) != heldMouseButtons.end()) {
                return true;
            }
            break;
        case IH_KEY_KEYBOARD:
            if (std::find(heldKeyboardKeys.begin(), heldKeyboardKeys.end(), key) != heldKeyboardKeys.end()) {
                return true;
            }
            break;
        case IH_KEY_CONTROLLER:
            if (std::find(heldControllerKeys.begin(), heldControllerKeys.end(), key) != heldControllerKeys.end()) {
                return true;
            }
            break;
    }
    return false;
}

bool InputHelper::KeyUp(int key, int type)
{
    for (SDL_Event e : sdlEvents) {
        if ( e.type == SDL_CONTROLLERBUTTONUP &&
             e.cbutton.button == (SDL_GameControllerButton) key &&
             type == IH_KEY_CONTROLLER ) {
            return true;
        }
        if ( e.type == SDL_KEYUP &&
             e.key.keysym.sym == (SDL_KeyCode) key &&
             type == IH_KEY_KEYBOARD ) {
            return true;
        }
        if ( e.type == SDL_MOUSEBUTTONUP && type == IH_KEY_MOUSE ) {
            SDL_MouseButtonEvent mbe = (SDL_MouseButtonEvent) e.button;
            if (mbe.button == key) {
                return true;
            }
        }
    }
    return false;
}

touchPosition& InputHelper::TouchRead()
{
	return mTouch;
}

