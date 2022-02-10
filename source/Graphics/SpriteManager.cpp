#include "SpriteManager.h"
#include <algorithm>

SpriteManager::SpriteManager() = default;

SpriteManager::~SpriteManager() {
	for (auto & mSprite : mSprites) {
		delete mSprite;
	}
    mSprites.clear();
    mSpritesSorted.clear();
}

void SpriteManager::SortZ() {
    mSpritesSorted = mSprites;
    std::sort(mSpritesSorted.begin(), mSpritesSorted.end(), [](const pDrawable* first, const pDrawable* second) {
        return first->Z > second->Z;
    });
}

void SpriteManager::Draw() {
    for (auto it = mSprites.begin(); it != mSprites.end();) {
        pDrawable *spr = *it;

        //if for some reason sprite is nonexistent then mark for deletion
        if (spr == nullptr) {
            it = mSprites.erase(it);
            continue;
        }

        spr->Update();

        //if sprite is dead then mark for deletion
        if (!spr->Alive()) {
            delete *it;
            it = mSprites.erase(it);
            continue;
        }

        ++it;
    }

	SortZ();

	for (auto it = mSpritesSorted.begin(); it != mSpritesSorted.end();) {
        pDrawable *spr = *it;

       //no need to draw things you can't see
        if (spr->Width == 0 || spr->Height == 0 || spr->Alpha == 0) {
            ++it;
            continue;
        }

		spr->Draw();
        ++it;
	}

	this->HandleTouchInput();
}

void SpriteManager::Add(pDrawable* spr) {
	mSprites.push_back(spr);
}

void SpriteManager::Add(const std::vector<pDrawable*>& spr) {
	for (auto it : spr) {
		Add(it);
	}
}

void SpriteManager::Remove(int32_t id) {
    if (mSprites.size() > id) {
        mSprites.erase(mSprites.begin() + id);
    } else {
        fprintf(stderr, "[WARNING]: SpriteManager was asked to remove out-of-bounds sprite.");
    }
}

void SpriteManager::RemoveLast() {
    mSprites.pop_back();
}

void SpriteManager::HandleTouchInput() {
    if (!InputHelper::KeyDown(SDL_BUTTON_LEFT, IH_KEY_MOUSE))
		return;
	touchPosition touchPos = InputHelper::TouchRead();

    SortZ();
    int i = 0;
    for (auto current : mSpritesSorted) {
        if (current == nullptr) {
            fprintf(stderr, "Null %i\n", i);
            i++;
            continue;
        }
        if (current->InBounds(touchPos.px, touchPos.py) && current->Clickable) {
			current->OnClick(current, touchPos.px, touchPos.py);
		}
	}
}