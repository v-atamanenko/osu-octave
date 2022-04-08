#include "InputHelper.h"

std::vector<SDL_Event> InputHelper::simulatedKeyDowns;

std::vector<RawKey> InputHelper::blockedKeys;
std::vector<RawKey> InputHelper::uppedKeys;
std::vector<RawKey> InputHelper::downedKeys;

touchPosition InputHelper::mTouch;

std::map<Control, std::vector<RawKey>> InputHelper::mControls;
bool InputHelper::vitaUseBackTouch;

inline bool InputHelper::checkKeyState(std::vector<RawKey>& v, const RawKey& k) {
    for (const auto& rk : v) {
        if (rk.type == k.type && rk.key == k.key) {
            return true;
        }
    }
    return false;
}

inline void InputHelper::releaseKey(const RawKey& k) {
    uppedKeys.push_back(k);

    for (auto it = blockedKeys.begin(); it != blockedKeys.end();) {
        RawKey rk = *it;

        if (rk.type == k.type && rk.key == k.key) {
            it = blockedKeys.erase(it);
            continue;
        }

        it++;
    }
}

inline void InputHelper::holdKey(const RawKey& k) {
    if (!checkKeyState(blockedKeys, k)) {
        downedKeys.push_back(k);
        blockedKeys.push_back(k);
    }
}


int InputHelper::PollSDL() {
    SDL_Event event;

    downedKeys.clear();
    uppedKeys.clear();

    while ( SDL_PollEvent(&event) ) {
        switch (event.type) {
            case SDL_QUIT:
                return -1;
            case SDL_CONTROLLERBUTTONDOWN:
                holdKey({event.cbutton.button, IH_KEY_CONTROLLER});
                break;
            case SDL_CONTROLLERBUTTONUP:
                releaseKey({event.cbutton.button, IH_KEY_CONTROLLER});
                break;
            case SDL_KEYDOWN:
                holdKey({event.key.keysym.sym, IH_KEY_KEYBOARD});
                break;
            case SDL_KEYUP:
                releaseKey({event.key.keysym.sym, IH_KEY_KEYBOARD});
                break;
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                if (event.type == SDL_MOUSEMOTION) {
                    mTouch.px = event.motion.x;
                    mTouch.py = event.motion.y;
                } else {
                    mTouch.px = event.button.x;
                    mTouch.py = event.button.y;
                }

                if (event.type == SDL_MOUSEBUTTONUP) {
                    releaseKey({event.button.button, IH_KEY_MOUSE});
                } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    holdKey({event.button.button, IH_KEY_MOUSE});
                }
                break;
            case SDL_FINGERMOTION:
            case SDL_FINGERDOWN:
            case SDL_FINGERUP:
                if ((!IS_PLATFORM_VITA) ||
                    (IS_PLATFORM_VITA && event.tfinger.touchId == 0 && !vitaUseBackTouch) ||
                    (IS_PLATFORM_VITA && event.tfinger.touchId == 1 && vitaUseBackTouch)) {
                    mTouch.px = (OOInt) round(event.tfinger.x * (OOFloat) SCREEN_WIDTH);
                    mTouch.py = (OOInt) round(event.tfinger.y * (OOFloat) SCREEN_HEIGHT);
                }

                // Handle touch down / touch up only if front touch is used.
                if ((!IS_PLATFORM_VITA) || (IS_PLATFORM_VITA && event.tfinger.touchId == 0 && !vitaUseBackTouch)) {
                    if (event.type == SDL_FINGERUP) {
                        releaseKey({SDL_BUTTON_LEFT, IH_KEY_MOUSE});
                    } else if (event.type == SDL_FINGERDOWN) {
                        holdKey({SDL_BUTTON_LEFT, IH_KEY_MOUSE});
                    }
                }
                break;
        }
    }

    SimulateKeyUp();
    return 0;
}

bool InputHelper::KeyDown(Control c) {
    for (RawKey k : mControls[c]) {
        if (checkKeyState(downedKeys, k)) return true;
    }
    return false;
}

bool InputHelper::KeyHeld(Control c) {
    for (RawKey k : mControls[c]) {
        if (checkKeyState(blockedKeys, k)) return true;
    }
    return false;
}

bool InputHelper::KeyUp(Control c) {
    for (RawKey k : mControls[c]) {
        if (checkKeyState(uppedKeys, k)) return true;
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

void InputHelper::InitInput() {
    //ClearInput();
    Settings::get_controls(mControls, vitaUseBackTouch);
}
