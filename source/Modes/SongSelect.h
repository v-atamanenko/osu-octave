#pragma once

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
#include "UIElements/Logo.h"

class SongSelect : public Mode
{
	public:
		SongSelect();
        ~SongSelect() override {
            delete mLogo;
        }

		void Update() override;
		void HandleInput() override;

        static void PageNext() {
            AudioManager::Engine().PlayUISound(UISOUND_CLICK_SHORT_CONFIRM);
            if (mCountPages == 0) { return; }
            if (mCurrentPage < (mCountPages - 1)) {
                PreviewBuffer::GetInstance().Update((OOInt)mCurrentPage, (mCurrentPage+1), mEntriesPerPage);
                mCurrentPage++;
                shouldHandlePageUpdate = true;
                Settings::set_int("page", (OOInt)mCurrentPage);
            }
        };
        static void PagePrev() {
            AudioManager::Engine().PlayUISound(UISOUND_CLICK_SHORT_CONFIRM);
            if (mCountPages == 0) { return; }
            if (mCurrentPage > 0) {
                PreviewBuffer::GetInstance().Update((OOInt)mCurrentPage, (mCurrentPage-1), mEntriesPerPage);
                mCurrentPage--;
                shouldHandlePageUpdate = true;
                Settings::set_int("page", (OOInt)mCurrentPage);
            }
        };
        static void PageRand() {
            AudioManager::Engine().PlayUISound(UISOUND_CLICK_SHORT_CONFIRM);
            if (mCountPages == 0) { return; }

            if (mCountPages <= 1) {
                shouldExpandRandomEntry = true;
                return;
            }

            auto p = (OOUInt) MathHelper::Random(0, (mCountPages - 1));
            if (p == mCurrentPage) {
                while (p == mCurrentPage) {
                    p = MathHelper::Random(0, (mCountPages - 1));
                }
            }

            PreviewBuffer::GetInstance().Update((OOInt)mCurrentPage, p, mEntriesPerPage);
            mCurrentPage = p;
            shouldHandlePageUpdate = true;
            shouldExpandRandomEntry = true;
            Settings::set_int("page", (OOInt)mCurrentPage);
        };
        static void ExpandEntry(OOUInt index) {
            shouldHandlePageUpdate = shouldHandleEntryExpand = true;
            if (mExpandEntryIndex == index) {
                AudioManager::Engine().PlayUISound(UISOUND_CLICK_CLOSE);
                mExpandEntryIndex = -1; // Second tap on expanded entry should close it.
            } else {
                AudioManager::Engine().PlayUISound(UISOUND_MENUCLICK);
                mExpandEntryIndex = index;
            }
        };
        static void ApplyFilter(BeatmapFilter f, bool resetpage=true);
        static void Redraw();

        static void LoadingScreenShow();
	
	protected:
        void UpdateSonglist(bool initial=false);
        void reloadPreviews() const;

        Logo* mLogo;

		static SpriteManager* mSpriteManager;

        static OOUInt mSongListSize;
        bool mEntryExpanded = false;
        static OOUInt mCurrentPage;
        static OOUInt mCountPages;
        static OOUInt mExpandEntryIndex;
        static bool shouldHandlePageUpdate;
        static bool shouldHandleEntryExpand;
        static bool shouldExpandRandomEntry;
        static const OOUInt mEntriesPerPage = 4;
        OOUInt mEntriesDisplayed = 0;

        OOUInt mSpritesPerBeatmapEntry = 8;
        OOUInt mSpritesPerExpandedBeatmapEntry = 15;

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
