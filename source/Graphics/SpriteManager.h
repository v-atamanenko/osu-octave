#pragma once

#include <vector>
#include <algorithm>

#include "pSprite.h"
#include "GraphicsManager.h"
#include "Helpers/InputHelper.h"

class SpriteManager
{
	public:
		SpriteManager() = default;
		virtual ~SpriteManager();

		void Draw(bool ignoreInput = false);
		void HandleTouchInput();

		void Add(pDrawable* spr);
		void Add(const std::vector<pDrawable*>& spr);

        void SortZ();

        void Clear();
        [[maybe_unused]] void Remove(OOUInt id);
        void RemoveLast();

        [[maybe_unused]] std::vector<pDrawable*>& Sprites() { return mSprites; }

	protected:
		std::vector<pDrawable*> mSprites;
        std::vector<pDrawable*> mSpritesSorted;
};
