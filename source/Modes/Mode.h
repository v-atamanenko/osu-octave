#include <cstdio>
#include <unistd.h>
#include "Helpers/InputHelper.h"

#ifndef __MODE_H__
#define __MODE_H__

typedef enum {
	MODE_PLAYER,
	MODE_SONGSELECT,
    MODE_WELCOME,
    MODE_SETTINGS,
    MODE_ABOUT
} ModeType;

class Mode
{
	public:
		virtual ~Mode();
		
		static Mode& CurrentMode() { return *sCurrentMode; }
		friend void ChangeModeOnFrameEnd(ModeType mode);
        friend void ChangeModeOnDemand();
		
		virtual void Update() = 0;
		virtual void HandleInput() = 0;

        static bool wantChangeMode;
        static ModeType wantChangeModeTo;
	protected:
		Mode() {  wantChangeMode = false; }
	
	private:
		static Mode* sCurrentMode;
};

void ChangeModeOnFrameEnd(ModeType mode);
void ChangeModeOnDemand();

#endif
