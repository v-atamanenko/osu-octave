#include <nds.h>
#include <vector>

#include "pSprite.h"
#include "GraphicsManager.h"
#include "Helpers/InputHelper.h"

#ifndef __SPRITEMANAGER_H__
#define __SPRITEMANAGER_H__



typedef std::vector<pSprite*>::iterator spriteIterator;
typedef std::vector<pSprite*>::const_iterator spriteIteratorConst;

class SpriteManager
{
	public:
		SpriteManager();
		virtual ~SpriteManager();
		
		void Draw();
		void HandleTouchInput();
		
		void Add(pSprite* spr);
		void Add(const std::vector<pSprite*>& spr);
		
		std::vector<pSprite*>& Sprites() { return mSprites; }
	
	protected:
		std::vector<pSprite*> mSprites;
};

#endif

