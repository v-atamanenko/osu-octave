#include "InputHelper.h"

std::vector<SDL_Event> InputHelper::sdlEvents;
std::vector<SDL_Event> InputHelper::simulatedKeyDowns;
std::vector<OOInt> InputHelper::heldControllerKeys;
std::vector<OOInt> InputHelper::heldKeyboardKeys;
std::vector<OOInt> InputHelper::heldMouseButtons;
touchPosition InputHelper::mTouch;
bool InputHelper::BlockKeydown = false;

std::map<Control, std::vector<RawKey>> InputHelper::mControls;
bool InputHelper::vitaUseBackTouch;

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
                BlockKeydown = false;
                break;
            case SDL_KEYDOWN:
                heldKeyboardKeys.push_back(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                heldKeyboardKeys.erase(std::remove(heldKeyboardKeys.begin(), heldKeyboardKeys.end(), event.key.keysym.sym), heldKeyboardKeys.end());
                BlockKeydown = false;
                break;
#ifndef __vita__ // VITA: Ignore mouse events, process touch events instead.
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
                BlockKeydown = false;
                break;
            case SDL_MOUSEMOTION:
                mme = (SDL_MouseMotionEvent) event.motion;
                mTouch.px = mme.x;
                mTouch.py = mme.y;
                break;
#endif
            case SDL_FINGERMOTION:
#ifdef __vita__
                if ((event.tfinger.touchId == 0 && !vitaUseBackTouch) ||
                    (event.tfinger.touchId == 1 && vitaUseBackTouch)) {
#endif
                    mTouch.px = (OOInt) round(event.tfinger.x * (OOFloat) SCREEN_WIDTH);
                    mTouch.py = (OOInt) round(event.tfinger.y * (OOFloat) SCREEN_HEIGHT);
#ifdef __vita__
                }
#endif
                break;
            case SDL_FINGERDOWN:
            case SDL_FINGERUP:
#ifdef __vita__
                if ((event.tfinger.touchId == 0 && !vitaUseBackTouch) ||
                    (event.tfinger.touchId == 1 && vitaUseBackTouch)) {
#endif
                mTouch.px = (OOInt) round(event.tfinger.x * (OOFloat) SCREEN_WIDTH);
                mTouch.py = (OOInt) round(event.tfinger.y * (OOFloat) SCREEN_HEIGHT);
#ifdef __vita__
                }
#endif
#ifdef __vita__
                // Handle touch down / touch up only if front touch is used.
                if (event.tfinger.touchId == 0 && !vitaUseBackTouch) {
#endif
                    if (event.type == SDL_FINGERDOWN) {
                        heldMouseButtons.push_back(SDL_BUTTON_LEFT);
                    } else {
                        heldMouseButtons.erase(std::remove(heldMouseButtons.begin(), heldMouseButtons.end(), SDL_BUTTON_LEFT), heldMouseButtons.end());
                        BlockKeydown = false;
                    }
#ifdef __vita__
                }
#endif
                break;
        }
    }

    SimulateKeyUp();
    return 0;
}

bool InputHelper::KeyDown(Control c) {
    for (SDL_Event e : sdlEvents) {
        for (RawKey k : mControls[c]) {
            if (e.type == SDL_CONTROLLERBUTTONDOWN &&
                e.cbutton.button == (SDL_GameControllerButton) k.key &&
                k.type == IH_KEY_CONTROLLER) {
                return true;
            }
            if (e.type == SDL_KEYDOWN &&
                e.key.keysym.sym == (SDL_KeyCode) k.key &&
                k.type == IH_KEY_KEYBOARD) {
                return true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && k.type == IH_KEY_MOUSE) {
#ifndef __vita__ // VITA: Ignore mouse events, process touch events instead.
                auto mbe = (SDL_MouseButtonEvent) e.button;
                if (mbe.button == k.key) {
                    return true;
                }
#endif
            }
            if (e.type == SDL_FINGERDOWN && k.type == IH_KEY_MOUSE) {
#ifdef __vita__
                // Handle touch down / touch up only if front touch is used.
                if (e.tfinger.touchId == 0 && !vitaUseBackTouch) {
#endif
                return true;
#ifdef __vita__
                }
#endif
            }
        }
    }
	return false;
}

bool InputHelper::KeyHeld(Control c)
{
    for (RawKey k : mControls[c]) {
        switch (k.type) {
            case IH_KEY_MOUSE:
                if (std::find(heldMouseButtons.begin(), heldMouseButtons.end(), k.key) != heldMouseButtons.end()) {
                    return true;
                }
                break;
            case IH_KEY_KEYBOARD:
                if (std::find(heldKeyboardKeys.begin(), heldKeyboardKeys.end(), k.key) != heldKeyboardKeys.end()) {
                    return true;
                }
                break;
            case IH_KEY_CONTROLLER:
                if (std::find(heldControllerKeys.begin(), heldControllerKeys.end(), k.key) != heldControllerKeys.end()) {
                    return true;
                }
                break;
        }
    }
    return false;
}

bool InputHelper::KeyUp(Control c)
{
    for (SDL_Event e : sdlEvents) {
        for (RawKey k : mControls[c]) {
            if (e.type == SDL_CONTROLLERBUTTONUP &&
                e.cbutton.button == (SDL_GameControllerButton) k.key &&
                k.type == IH_KEY_CONTROLLER) {
                return true;
            }
            if (e.type == SDL_KEYUP &&
                e.key.keysym.sym == (SDL_KeyCode) k.key &&
                k.type == IH_KEY_KEYBOARD) {
                return true;
            }
            if (e.type == SDL_MOUSEBUTTONUP && k.type == IH_KEY_MOUSE) {
#ifndef __vita__ // VITA: Ignore mouse events, process touch events instead.
                auto mbe = (SDL_MouseButtonEvent) e.button;
                if (mbe.button == k.key) {
                    return true;
                }
#endif
            }
            if (e.type == SDL_FINGERUP && k.type == IH_KEY_MOUSE) {
#ifdef __vita__
                // Handle touch down / touch up only if front touch is used.
                if (e.tfinger.touchId == 0 && !vitaUseBackTouch) {
#endif
                return true;
#ifdef __vita__
                }
#endif
            }
        }
    }
    return false;
}

void InputHelper::SimulateKeyDown(Control c) {
    SDL_Event e;
    RawKey k = mControls[c][0]; // Use whatever RawKey for Control

    if ( k.type == IH_KEY_CONTROLLER ) {
        e.type = SDL_CONTROLLERBUTTONDOWN;
        e.cbutton.button = (SDL_GameControllerButton) k.key;
    }
    if ( k.type == IH_KEY_KEYBOARD ) {
        e.type = SDL_KEYDOWN;
        e.key.keysym.sym = (SDL_KeyCode) k.key;
    }
    if ( k.type == IH_KEY_MOUSE ) {
        e.type = SDL_MOUSEBUTTONDOWN;
        SDL_MouseButtonEvent mbe;
        mbe.button = k.key;
        e.button = mbe;
    }
    simulatedKeyDowns.push_back(e);
    SDL_PushEvent(&e);
}

void InputHelper::SimulateKeyUp() {
    for (auto it = simulatedKeyDowns.begin(); it != simulatedKeyDowns.end();) {
        SDL_Event e = *it;
        SDL_Event ep;
        ep = e;

        if (e.type == SDL_CONTROLLERBUTTONDOWN) {
            ep.type = SDL_CONTROLLERBUTTONUP;
        }
        if (e.type == SDL_KEYDOWN) {
            ep.type = SDL_KEYUP;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            ep.type = SDL_MOUSEBUTTONDOWN;
        }

        SDL_PushEvent(&ep);
        it = simulatedKeyDowns.erase(it);
    }
}

touchPosition& InputHelper::TouchRead() {
	return mTouch;
}

void InputHelper::ClearInput() {
    sdlEvents.clear();
    simulatedKeyDowns.clear();
    heldControllerKeys.clear();
    heldKeyboardKeys.clear();
    heldMouseButtons.clear();
    BlockKeydown = false;
}

void InputHelper::InitInput() {
    //ClearInput();
    BlockKeydown = true;
    Settings::get_controls(mControls, vitaUseBackTouch);
    BlockKeydown = false;
}
