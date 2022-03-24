#include <vector>

#include "pSprite.h"
#include "GraphicsManager.h"
#include "Helpers/InputHelper.h"

#ifndef __SPRITEMANAGER_H__
#define __SPRITEMANAGER_H__

typedef std::vector<pDrawable*>::iterator spriteIterator;
typedef std::vector<pDrawable*>::const_iterator spriteIteratorConst;

class SpriteManager
{
	public:
		SpriteManager();
		virtual ~SpriteManager();
		
		void Draw(bool ignoreInput = false);
		void HandleTouchInput();
        void SortZ();
		
		void Add(pDrawable* spr);
		void Add(const std::vector<pDrawable*>& spr);
        void Remove(int32_t id);
        void RemoveLast();

        void Clear();
		
		std::vector<pDrawable*>& Sprites() { return mSprites; }
	
	protected:
		std::vector<pDrawable*> mSprites;
        std::vector<pDrawable*> mSpritesSorted;
};

#endif

