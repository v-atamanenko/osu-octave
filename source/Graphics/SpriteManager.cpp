#include "SpriteManager.h"

SpriteManager::~SpriteManager() {
	for (auto & mSprite : mSprites) {
		delete mSprite;
	}
    mSprites.clear();
    mSpritesSorted.clear();
}

void SpriteManager::SortZ() {
    mSpritesSorted.clear();
    mSpritesSorted = mSprites;
    std::sort(mSpritesSorted.begin(), mSpritesSorted.end(), [](const pDrawable* first, const pDrawable* second) {
        return first->Z > second->Z;
    });
}

void SpriteManager::Draw(bool ignoreInput) {
    for (auto it = mSprites.begin(); it != mSprites.end();) {
        pDrawable *spr = *it;

        // If for some reason the sprite is null, remove it.
        if (spr == nullptr) {
            it = mSprites.erase(it);
            continue;
        }

        spr->Update();

        // If sprite is dead after the update, remove it.
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

        // No need to draw things you can't see.
        if (spr->Width == 0 || spr->Height == 0 || spr->Alpha == 0) {
            ++it;
            continue;
        }

		spr->Draw();
        ++it;
	}

    if (!ignoreInput) {
        this->HandleTouchInput();
    }
}

void SpriteManager::Add(pDrawable* spr) {
	mSprites.push_back(spr);
}

void SpriteManager::Add(const std::vector<pDrawable*>& spr) {
	for (auto it : spr) {
		Add(it);
	}
}

[[maybe_unused]] void SpriteManager::Remove(OOUInt id) {
    if (mSprites.size() > id) {
        mSprites.erase(mSprites.begin() + id);
    } else {
        fprintf(stderr, "[WARNING]: SpriteManager was asked to remove an out-of-bounds sprite.\n");
    }
}

void SpriteManager::RemoveLast() {
    delete mSprites.back();
    mSprites.pop_back();
}

void SpriteManager::Clear() {
    for (auto spr : mSprites) {
        delete spr;
    }

    mSprites.clear();
}

void SpriteManager::HandleTouchInput() {
    if (!InputHelper::KeyDown(Control::IH_CONTROL_ACTION)) return;
	touchPosition touchPos = InputHelper::TouchRead();

    SortZ();
    for (auto current : mSpritesSorted) {
        if (current->Clickable) {
            if (current->InBounds(touchPos.px, touchPos.py)) {
                current->OnClick(current, touchPos.px, touchPos.py);
            }
        }
	}
}
