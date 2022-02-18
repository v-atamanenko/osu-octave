#include "SDL.h"
#include <cstdio>

#include "Mode.h"
#include "Beatmaps/BeatmapManager.h"
#include "Graphics/SpriteManager.h"
#include "System/TextManager.h"
#include "Helpers/PreviewBuffer.h"
#include "System/Settings.h"

#ifndef __SONGSELECT_H__
#define __SONGSELECT_H__

class SongSelect : public Mode
{
	public:
		SongSelect();
		
		void Update();
		void HandleInput();

        static void PageNext() {
            if (mCurrentPage < (mCountPages - 1)) {
                PreviewBuffer::GetInstance().Update(mCurrentPage, (mCurrentPage+1), mEntriesPerPage);
                mCurrentPage++;
                shouldHandlePageUpdate = true;
                Settings::set_int("page", mCurrentPage);
            }
        };
        static void PagePrev() {
            if (mCurrentPage > 0) {
                PreviewBuffer::GetInstance().Update(mCurrentPage, (mCurrentPage-1), mEntriesPerPage);
                mCurrentPage--;
                shouldHandlePageUpdate = true;
                Settings::set_int("page", mCurrentPage);
            }
        };
        static void ExpandEntry(int index) {
            shouldHandlePageUpdate = shouldHandleEntryExpand = true;
            if (mExpandEntryIndex == index) {
                mExpandEntryIndex = -1; // Second tap on expanded entry should close it.
            } else {
                mExpandEntryIndex = index;
            }
        };
	
	protected:
        void UpdateSonglist();
        void reloadPreviews() const;

		SpriteManager mSpriteManager;

        int32_t mSongListSize;
        bool mEntryExpanded = false;
        static int32_t mCurrentPage;
        static int32_t mCountPages;
        static int32_t mExpandEntryIndex;
        static bool shouldHandlePageUpdate;
        static bool shouldHandleEntryExpand;
        static const int32_t mEntriesPerPage = 4;
        int32_t mEntriesDisplayed = 0;

        int32_t mSpritesPerBeatmapEntry = 7;
        int32_t mSpritesPerExpandedBeatmapEntry = 13;
};

#endif

