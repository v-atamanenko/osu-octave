#include <cstdio>
#include <unistd.h>
#include "Helpers/InputHelper.h"

#ifndef __MODE_H__
#define __MODE_H__

typedef enum {
	MODE_PLAYER,
	MODE_SONGSELECT
} ModeType;

class Mode
{
	public:
		virtual ~Mode() {}
		
		static Mode& CurrentMode() { return *sCurrentMode; }
		friend void ChangeMode(ModeType mode);
		
		virtual void Update() = 0;
		virtual void HandleInput() = 0;
		
		static void ChangeToOsuDir();
	
	protected:
		Mode() { ChangeToOsuDir(); } //reset directory for new mode
	
	private:
		static Mode* sCurrentMode;
};

void ChangeMode(ModeType mode);

#endif


