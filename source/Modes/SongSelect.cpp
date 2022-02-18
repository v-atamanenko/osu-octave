#include "SongSelect.h"


#include "Graphics/pText.h"

int32_t SongSelect::mCurrentPage = 0;
int32_t SongSelect::mCountPages  = 0;
bool SongSelect::shouldHandlePageUpdate = false;

int32_t SongSelect::mExpandEntryIndex = -1;
bool SongSelect::shouldHandleEntryExpand = false;
//bool SongSelect::mEntryExpanded = false;

void OnBetmapEntryPlayClick(pDrawable* self, uint16_t x, uint16_t y) {
	int index = (int)(size_t) self->Tag;

	BeatmapManager::Load(index);
    ChangeModeOnFrameEnd(MODE_PLAYER);
}

void OnBetmapEntryExpandClick(pDrawable* self, uint16_t x, uint16_t y) {
    int index = (int)(size_t) self->Tag;

    SongSelect::ExpandEntry(index);
}

void OnBtnQuitClick(pDrawable* self, uint16_t x, uint16_t y) {
    SDL_Event sdlevent;
    sdlevent.type = SDL_QUIT;
    SDL_PushEvent(&sdlevent);
}

void OnBtnRandomClick(pDrawable* self, uint16_t x, uint16_t y) {
    // X
}

void OnBtnArrowLeftClick(pDrawable* self, uint16_t x, uint16_t y) {
    SongSelect::PagePrev();
}

void OnBtnArrowRightClick(pDrawable* self, uint16_t x, uint16_t y) {
    SongSelect::PageNext();
}

void OnBtnSortAllClick(pDrawable* self, uint16_t x, uint16_t y) {
    // X
}

void OnBtnSortAEClick(pDrawable* self, uint16_t x, uint16_t y) {
    // X
}

void OnBtnSortFJClick(pDrawable* self, uint16_t x, uint16_t y) {
    // X
}

void OnBtnSortKOClick(pDrawable* self, uint16_t x, uint16_t y) {
    // X
}

void OnBtnSortPTClick(pDrawable* self, uint16_t x, uint16_t y) {
    // X
}

void OnBtnSortUZClick(pDrawable* self, uint16_t x, uint16_t y) {
    // X
}

SongSelect::SongSelect()
{
    GraphicsManager::Graphics().LoadTexturesForMode(MODE_SONGSELECT);
    PreviewBuffer::GetInstance().Update(-1, 0, mEntriesPerPage);

    pSprite* bg = new pSprite(TX_SONGSELECT_BG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255);
    mSpriteManager.Add(bg);

    pSprite* btn_about = new pSprite(TX_BUTTON_BIG, 37, 281, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    mSpriteManager.Add(btn_about);
    pText* btn_about_label = new pText("About", FONT_PIXEL, 175, 308, SDL_Color({67,19,115}));
    btn_about_label->Z = -0.02f;
    btn_about_label->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(btn_about_label);

    pSprite* btn_settings = new pSprite(TX_BUTTON_BIG, 37, 349, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    mSpriteManager.Add(btn_settings);
    pText* btn_settings_label = new pText("Settings", FONT_PIXEL, 175, 376, SDL_Color({67,19,115}));
    btn_settings_label->Z = -0.02f;
    btn_settings_label->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(btn_settings_label);

    pSprite* btn_quit = new pSprite(TX_BUTTON_BIG, 37, 418, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_quit->OnClick = OnBtnQuitClick;
    btn_quit->Clickable = true;
    mSpriteManager.Add(btn_quit);
    pText* btn_quit_label = new pText("Quit", FONT_PIXEL, 175, 445, SDL_Color({67,19,115}));
    btn_quit_label->Z = -0.02f;
    btn_quit_label->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(btn_quit_label);

    pSprite* btn_random = new pSprite(TX_BUTTON_MED, 400, 478, 207, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_random->OnClick = OnBtnRandomClick;
    btn_random->Clickable = true;
    mSpriteManager.Add(btn_random);
    pText* btn_random_label = new pText("Random", FONT_PIXEL, 503, 498, SDL_Color({67,19,115}));
    btn_random_label->Z = -0.02f;
    btn_random_label->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(btn_random_label);

    pSprite* btn_arrow_left = new pSprite(TX_BUTTON_ARROW, (780-78), 478, 78, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_arrow_left->Angle = 180;
    btn_arrow_left->OnClick = OnBtnArrowLeftClick;
    btn_arrow_left->Clickable = true;
    mSpriteManager.Add(btn_arrow_left);

    pSprite* btn_arrow_right = new pSprite(TX_BUTTON_ARROW, 841, 478, 78, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_arrow_right->OnClick = OnBtnArrowRightClick;
    btn_arrow_right->Clickable = true;
    mSpriteManager.Add(btn_arrow_right);

    pSprite* btn_sort_all = new pSprite(TX_BUTTON_SM_ACTIVE, 400, 25, 78, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_all->OnClick = OnBtnSortAllClick;
    btn_sort_all->Clickable = true;
    mSpriteManager.Add(btn_sort_all);
    pText* btn_sort_all_label = new pText("ALL", FONT_PIXEL_ACTIVE, 439, 45, SDL_Color({255,255,255}));
    btn_sort_all_label->Z = -0.02f;
    btn_sort_all_label->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(btn_sort_all_label);

    pSprite* btn_sort_a_e = new pSprite(TX_BUTTON_XS, 536, 25, 67, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_a_e->OnClick = OnBtnSortAEClick;
    btn_sort_a_e->Clickable = true;
    mSpriteManager.Add(btn_sort_a_e);
    pText* btn_sort_a_e_label = new pText("A-E", FONT_PIXEL, 569, 45, SDL_Color({67,19,115}));
    btn_sort_a_e_label->Z = -0.02f;
    btn_sort_a_e_label->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(btn_sort_a_e_label);

    pSprite* btn_sort_f_j = new pSprite(TX_BUTTON_XS, 615, 25, 67, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_f_j->OnClick = OnBtnSortFJClick;
    btn_sort_f_j->Clickable = true;
    mSpriteManager.Add(btn_sort_f_j);
    pText* btn_sort_f_j_label = new pText("F-J", FONT_PIXEL, 648, 45, SDL_Color({67,19,115}));
    btn_sort_f_j_label->Z = -0.02f;
    btn_sort_f_j_label->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(btn_sort_f_j_label);

    pSprite* btn_sort_k_o = new pSprite(TX_BUTTON_XS, 694, 25, 67, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_k_o->OnClick = OnBtnSortKOClick;
    btn_sort_k_o->Clickable = true;
    mSpriteManager.Add(btn_sort_k_o);
    pText* btn_sort_k_o_label = new pText("K-O", FONT_PIXEL, 727, 45, SDL_Color({67,19,115}));
    btn_sort_k_o_label->Z = -0.02f;
    btn_sort_k_o_label->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(btn_sort_k_o_label);

    pSprite* btn_sort_p_t = new pSprite(TX_BUTTON_XS, 772, 25, 67, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_p_t->OnClick = OnBtnSortPTClick;
    btn_sort_p_t->Clickable = true;
    mSpriteManager.Add(btn_sort_p_t);
    pText* btn_sort_p_t_label = new pText("P-T", FONT_PIXEL, 806, 45, SDL_Color({67,19,115}));
    btn_sort_p_t_label->Z = -0.02f;
    btn_sort_p_t_label->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(btn_sort_p_t_label);

    pSprite* btn_sort_u_z = new pSprite(TX_BUTTON_XS, 852, 25, 67, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_u_z->OnClick = OnBtnSortUZClick;
    btn_sort_u_z->Clickable = true;
    mSpriteManager.Add(btn_sort_u_z);
    pText* btn_sort_u_z_label = new pText("U-Z", FONT_PIXEL, 885, 45, SDL_Color({67,19,115}));
    btn_sort_u_z_label->Z = -0.02f;
    btn_sort_u_z_label->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(btn_sort_u_z_label);

    //mBeatmapSpritesStartIndex = mSpriteManager.Sprites().size() + 1;
    mSongListSize = BeatmapManager::SongCount();
    mCurrentPage = Settings::get_int("page");
    mCountPages  = ceil(((float)mSongListSize/(float)mEntriesPerPage));

    UpdateSonglist();
}

void SongSelect::UpdateSonglist()
{
    int spritesToRemoveCount = mSpritesPerBeatmapEntry * mEntriesDisplayed;
    if (mEntryExpanded) spritesToRemoveCount += (mSpritesPerExpandedBeatmapEntry - mSpritesPerBeatmapEntry);
    if (spritesToRemoveCount != 0) spritesToRemoveCount++; // to account for current_page_label
    for (int i = 0; i < spritesToRemoveCount; ++i) {
        mSpriteManager.RemoveLast();
    }
    mEntryExpanded = false;
    mEntriesDisplayed = 0;

    if (shouldHandleEntryExpand && mExpandEntryIndex == -1) {
        shouldHandleEntryExpand = false;
    }

    int beatmap_list_offset = mCurrentPage * mEntriesPerPage;
    int entries_on_page = mEntriesPerPage;
    if (beatmap_list_offset + (entries_on_page-1) >= mSongListSize) {
        entries_on_page = mSongListSize - (beatmap_list_offset);
    }

    // Skip drawing first or last entry if we need to expand an entry and there's not enough free space
    bool skipFirstEntry = false;
    bool skipLastEntry = false;

    if (shouldHandleEntryExpand && entries_on_page == mEntriesPerPage) {
        // First entry. Hide if:
        if ((mExpandEntryIndex == (beatmap_list_offset+entries_on_page-1)) // entry to expand is the last...
            || (mExpandEntryIndex == (beatmap_list_offset+entries_on_page-2))) // or second to last entry.
        {
            skipFirstEntry = true;
        }

        // Last entry. Hide if:
        if ((mExpandEntryIndex == beatmap_list_offset) // entry to expand is the first...
            || mExpandEntryIndex == beatmap_list_offset+1) // or second entry.
        {
            skipLastEntry = true;
        }
    }

    for (int i = 0; i < entries_on_page; i++) {
        if ((i == 0 && skipFirstEntry) || (i == (entries_on_page-1) && skipLastEntry)) {
            continue;
        }

        bool isExpanded = (shouldHandleEntryExpand && (mExpandEntryIndex == (i+beatmap_list_offset)));
        TextureType texid;

        switch (i) {
            case 0: {
                texid = TX_BEATMAP_ENTRY_PIC_0;
                break;
            }
            case 1: {
                texid = TX_BEATMAP_ENTRY_PIC_1;
                break;
            }
            case 2: {
                texid = TX_BEATMAP_ENTRY_PIC_2;
                break;
            }
            case 3: {
                texid = TX_BEATMAP_ENTRY_PIC_3;
                break;
            }
            default: {
                fprintf(stderr, "[FATAL]: EntriesPerPage settings is higher than the amount of available texture buffers.\n");
                break;
            }
        }

        int extra_y_offset = 0;
        if (skipFirstEntry) {
            // if we hid the first entry, shift everything 94 pixels up
            extra_y_offset = -94;
        }
        if (shouldHandleEntryExpand && mExpandEntryIndex < (i+beatmap_list_offset)) {
            // if expanded entry was before this one, add extra Y offset
            extra_y_offset += 94;
        }

        Beatmap* map = BeatmapManager::Beatmaps()[i+beatmap_list_offset];

        GraphicsManager::Graphics().LoadBeatmapPicTexture(texid, PreviewBuffer::GetInstance().GetTexture(i+beatmap_list_offset));

        if (isExpanded) {
            pSprite* bg = new pSprite(TX_BEATMAP_ENTRY_EXPANDED_BG, 351, 91 + (i * 94) + extra_y_offset, 609, 174, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.03f);

            //bg->OnClick = OnBetmapEntryPlayClick;
            bg->OnClick = OnBetmapEntryExpandClick;
            bg->Clickable = true;
            bg->Tag = (void *) (size_t) (i + beatmap_list_offset);

            mSpriteManager.Add(bg);
        } else {
            pSprite* bg = new pSprite(TX_BEATMAP_ENTRY_BG, 351, 91 + (i * 94) + extra_y_offset, 609, 80, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.03f);

            bg->OnClick = OnBetmapEntryExpandClick;
            bg->Clickable = true;
            bg->Tag = (void *) (size_t) (i + beatmap_list_offset);

            mSpriteManager.Add(bg);
        }

        if (isExpanded) {
            pSprite* play = new pSprite(TX_BUTTON_PLAY, 831, 200 + (i * 94) + extra_y_offset, 78, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.04f);

            play->OnClick = OnBetmapEntryPlayClick;
            play->Clickable = true;
            play->Tag = (void *) (size_t) (i + beatmap_list_offset);

            mSpriteManager.Add(play);
        }

        if (isExpanded) {
            pSprite *pic = new pSprite(texid, 360, 118 + (i * 94) + extra_y_offset, 120, 120, ORIGIN_TOPLEFT, FIELD_SCREEN,
                                       SDL_Color(), 255, -0.04f);
            mSpriteManager.Add(pic);
        } else {
            pSprite *pic = new pSprite(texid, 400, 91 + (i * 94) + extra_y_offset, 80, 80, ORIGIN_TOPLEFT, FIELD_SCREEN,
                                       SDL_Color(), 255, -0.04f);
            mSpriteManager.Add(pic);
        }

        if (isExpanded) {
            pText *mapTitle = new pText(map->Title(), FONT_CONSOLE_BIG_BOLD, 494, 117 + (i * 94) + extra_y_offset);
            mapTitle->Z = -0.05f;

            pText *mapArtist = new pText(map->Artist(), FONT_CONSOLE, 494, 137 + (i * 94) + extra_y_offset);
            mapArtist->Z = -0.05f;

            pText *mapVersion = new pText(map->Version(), FONT_CONSOLE, 494, 154 + (i * 94) + extra_y_offset);
            mapVersion->Z = -0.05f;

            //TODO: Replace this with actual highscore data
            pText *highscoreTitle = new pText("Personal best:", FONT_CONSOLE_BIG_BOLD, 494, 184 + (i * 94) + extra_y_offset);
            highscoreTitle->Z = -0.06f;
            pText *highscoreScoreTitle = new pText("Score:", FONT_CONSOLE_BOLD, 494, 204 + (i * 94) + extra_y_offset);
            highscoreScoreTitle->Z = -0.06f;
            pText *highscoreScore = new pText("1.264.594 (240x)", FONT_CONSOLE, 540, 204 + (i * 94) + extra_y_offset);
            highscoreScore->Z = -0.06f;
            pText *highscoreAccuracyTitle = new pText("Accuracy:", FONT_CONSOLE_BOLD, 494, 222 + (i * 94) + extra_y_offset);
            highscoreAccuracyTitle->Z = -0.06f;
            pText *highscoreAccuracy = new pText("99.867%", FONT_CONSOLE, 563, 222 + (i * 94) + extra_y_offset);
            highscoreAccuracy->Z = -0.06f;

            mSpriteManager.Add(mapTitle);
            mSpriteManager.Add(mapArtist);
            mSpriteManager.Add(mapVersion);
            mSpriteManager.Add(highscoreTitle);
            mSpriteManager.Add(highscoreScoreTitle);
            mSpriteManager.Add(highscoreScore);
            mSpriteManager.Add(highscoreAccuracyTitle);
            mSpriteManager.Add(highscoreAccuracy);
        } else {
            pText *mapTitle = new pText(map->Title(), FONT_CONSOLE_BIG_BOLD, 494, 106 + (i * 94) + extra_y_offset);
            mapTitle->Z = -0.05f;

            pText *mapArtist = new pText(map->Artist(), FONT_CONSOLE, 494, 126 + (i * 94) + extra_y_offset);
            mapArtist->Z = -0.05f;

            pText *mapVersion = new pText(map->Version(), FONT_CONSOLE, 494, 143 + (i * 94) + extra_y_offset);
            mapVersion->Z = -0.05f;

            mSpriteManager.Add(mapTitle);
            mSpriteManager.Add(mapArtist);
            mSpriteManager.Add(mapVersion);
        }

        if (isExpanded) {
            //TODO: Replace this with actual ranking
            pSprite *ranking = new pSprite(TX_RANKING_UNK_SM, 869, 122 + (i * 94) + extra_y_offset, 40, 23,
                                           ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.04f);
            mSpriteManager.Add(ranking);

            //TODO: Replace this with actual star rating
            pSprite *stars = new pSprite(TX_STARS, 712, 161 + (i * 94) + extra_y_offset, 196, 16, ORIGIN_TOPLEFT,
                                         FIELD_SCREEN, SDL_Color(), 255, -0.04f);
            mSpriteManager.Add(stars);
        } else {
            //TODO: Replace this with actual ranking
            pSprite *ranking = new pSprite(TX_RANKING_UNK_SM, 869, 104 + (i * 94) + extra_y_offset, 40, 23,
                                           ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.04f);
            mSpriteManager.Add(ranking);

            //TODO: Replace this with actual star rating
            pSprite *stars = new pSprite(TX_STARS, 712, 141 + (i * 94) + extra_y_offset, 196, 16, ORIGIN_TOPLEFT,
                                         FIELD_SCREEN, SDL_Color(), 255, -0.04f);
            mSpriteManager.Add(stars);
        }

        mEntriesDisplayed++;
    }

    pText* current_page_label = new pText(std::to_string(mCurrentPage+1)+"/"+std::to_string(mCountPages), FONT_PIXEL, 811, 498, SDL_Color({67,19,115}));
    current_page_label->Z = -0.02f;
    current_page_label->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(current_page_label);

    if (shouldHandleEntryExpand) mEntryExpanded = true;
}

void SongSelect::reloadPreviews() const {
    int beatmap_list_offset = mCurrentPage * mEntriesPerPage;
    int entries_on_page = mEntriesPerPage;
    if (beatmap_list_offset + (entries_on_page-1) >= mSongListSize) {
        entries_on_page = mSongListSize - (beatmap_list_offset);
    }

    for (int i = 0; i < entries_on_page; i++) {
        TextureType texid;

        switch (i) {
            case 0: {
                texid = TX_BEATMAP_ENTRY_PIC_0;
                break;
            }
            case 1: {
                texid = TX_BEATMAP_ENTRY_PIC_1;
                break;
            }
            case 2: {
                texid = TX_BEATMAP_ENTRY_PIC_2;
                break;
            }
            case 3: {
                texid = TX_BEATMAP_ENTRY_PIC_3;
                break;
            }
            default: {
                fprintf(stderr, "[FATAL]: EntriesPerPage settings is higher than the amount of available texture buffers.\n");
                continue;
            }
        }

        GraphicsManager::Graphics().LoadBeatmapPicTexture(texid, PreviewBuffer::GetInstance().GetTexture(i+beatmap_list_offset));
    }
}

void SongSelect::HandleInput() {}

void SongSelect::Update()
{
    if (shouldHandlePageUpdate) {
        UpdateSonglist();
        shouldHandlePageUpdate = false;
        shouldHandleEntryExpand = false;
    }

    reloadPreviews();

	mSpriteManager.Draw();
}
