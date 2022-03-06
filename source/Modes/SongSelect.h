#include "SDL.h"
#include <cstdio>

#include "Mode.h"
#include "Beatmaps/BeatmapManager.h"
#include "Graphics/SpriteManager.h"
#include "System/TextManager.h"
#include "Helpers/PreviewBuffer.h"
#include "DataStorage/Settings.h"
#include "DataStorage/Scores.h"
#include "Graphics/pText.h"

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
        static void ApplyFilter(BeatmapFilter f, bool resetpage=true);
        static void Redraw();

        static void LoadingScreenShow();
        static void LoadingScreenHide();
	
	protected:
        void UpdateSonglist();
        void reloadPreviews() const;

		static SpriteManager* mSpriteManager;

        static uint32_t mSongListSize;
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

        static pSprite* btn_sort_all;
        static pText* btn_sort_all_label;
        static pSprite* btn_sort_a_e;
        static pText* btn_sort_a_e_label;
        static pSprite* btn_sort_f_j;
        static pText* btn_sort_f_j_label;
        static pSprite* btn_sort_k_o;
        static pText* btn_sort_k_o_label;
        static pSprite* btn_sort_p_t;
        static pText* btn_sort_p_t_label;
        static pSprite* btn_sort_u_z;
        static pText* btn_sort_u_z_label;

        static pSprite* loadingScreenBG;
        static pText* loadingScreenLabel;
};

#endif

