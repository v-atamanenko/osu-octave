#include "SongSelect.h"

#include "Graphics/pText.h"

int32_t SongSelect::mCurrentPage = 0;
int32_t SongSelect::mCountPages  = 0;
bool SongSelect::shouldHandlePageUpdate = false;

void OnMapSpriteClick(pDrawable* self, uint16_t x, uint16_t y) {
	int index = (int)(size_t) self->Tag;

	BeatmapManager::Load(index);
    ChangeModeOnFrameEnd(MODE_PLAYER);
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

    pSprite* bg = new pSprite(TX_SONGSELECT_BG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255);
    mSpriteManager.Add(bg);

    pSprite* btn_about = new pSprite(TX_BUTTON_BIG, 37, 281, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    mSpriteManager.Add(btn_about);
    pText* btn_about_label = new pText("about", FONT_PIXEL, 139, 302, SDL_Color({67,19,115}));
    btn_about_label->Z = -0.02f;
    mSpriteManager.Add(btn_about_label);

    pSprite* btn_settings = new pSprite(TX_BUTTON_BIG, 37, 349, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    mSpriteManager.Add(btn_settings);
    pText* btn_settings_label = new pText("settings", FONT_PIXEL, 119, 370, SDL_Color({67,19,115}));
    btn_settings_label->Z = -0.02f;
    mSpriteManager.Add(btn_settings_label);

    pSprite* btn_quit = new pSprite(TX_BUTTON_BIG, 37, 418, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_quit->OnClick = OnBtnQuitClick;
    btn_quit->Clickable = true;
    mSpriteManager.Add(btn_quit);
    pText* btn_quit_label = new pText("quit", FONT_PIXEL, 143, 440, SDL_Color({67,19,115}));
    btn_quit_label->Z = -0.02f;
    mSpriteManager.Add(btn_quit_label);

    pSprite* btn_random = new pSprite(TX_BUTTON_MED, 400, 478, 207, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_random->OnClick = OnBtnRandomClick;
    btn_random->Clickable = true;
    mSpriteManager.Add(btn_random);
    pText* btn_random_label = new pText("random", FONT_PIXEL, 462, 490, SDL_Color({67,19,115}));
    btn_random_label->Z = -0.02f;
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
    pText* btn_sort_all_label = new pText("ALL", FONT_PIXEL, 419, 38, SDL_Color({255,255,255}));
    btn_sort_all_label->Z = -0.02f;
    mSpriteManager.Add(btn_sort_all_label);

    pSprite* btn_sort_a_e = new pSprite(TX_BUTTON_XS, 536, 25, 67, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_a_e->OnClick = OnBtnSortAEClick;
    btn_sort_a_e->Clickable = true;
    mSpriteManager.Add(btn_sort_a_e);
    pText* btn_sort_a_e_label = new pText("A-E", FONT_PIXEL, 548, 38, SDL_Color({67,19,115}));
    btn_sort_a_e_label->Z = -0.02f;
    mSpriteManager.Add(btn_sort_a_e_label);

    pSprite* btn_sort_f_j = new pSprite(TX_BUTTON_XS, 615, 25, 67, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_f_j->OnClick = OnBtnSortFJClick;
    btn_sort_f_j->Clickable = true;
    mSpriteManager.Add(btn_sort_f_j);
    pText* btn_sort_f_j_label = new pText("F-J", FONT_PIXEL, 627, 38, SDL_Color({67,19,115}));
    btn_sort_f_j_label->Z = -0.02f;
    mSpriteManager.Add(btn_sort_f_j_label);

    pSprite* btn_sort_k_o = new pSprite(TX_BUTTON_XS, 694, 25, 67, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_k_o->OnClick = OnBtnSortKOClick;
    btn_sort_k_o->Clickable = true;
    mSpriteManager.Add(btn_sort_k_o);
    pText* btn_sort_k_o_label = new pText("K-O", FONT_PIXEL, 706, 38, SDL_Color({67,19,115}));
    btn_sort_k_o_label->Z = -0.02f;
    mSpriteManager.Add(btn_sort_k_o_label);

    pSprite* btn_sort_p_t = new pSprite(TX_BUTTON_XS, 772, 25, 67, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_p_t->OnClick = OnBtnSortPTClick;
    btn_sort_p_t->Clickable = true;
    mSpriteManager.Add(btn_sort_p_t);
    pText* btn_sort_p_t_label = new pText("P-T", FONT_PIXEL, 785, 38, SDL_Color({67,19,115}));
    btn_sort_p_t_label->Z = -0.02f;
    mSpriteManager.Add(btn_sort_p_t_label);

    pSprite* btn_sort_u_z = new pSprite(TX_BUTTON_XS, 852, 25, 67, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_sort_u_z->OnClick = OnBtnSortUZClick;
    btn_sort_u_z->Clickable = true;
    mSpriteManager.Add(btn_sort_u_z);
    pText* btn_sort_u_z_label = new pText("U-Z", FONT_PIXEL, 864, 38, SDL_Color({67,19,115}));
    btn_sort_u_z_label->Z = -0.02f;
    mSpriteManager.Add(btn_sort_u_z_label);

    mBeatmapSpritesStartIndex = mSpriteManager.Sprites().size();
    mSongListSize = BeatmapManager::SongCount();
    mCurrentPage = 0;
    mCountPages  = ceil(((float)mSongListSize/(float)mEntriesPerPage));

    UpdateSonglist();
}

void SongSelect::UpdateSonglist()
{
    int spritesToRemoveCount = mSpritesPerBeatmapEntry * mEntiesDisplayed;
    for (int i = 0; i < spritesToRemoveCount; ++i) {
        mSpriteManager.RemoveLast();
    }
    mEntiesDisplayed = 0;

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
                break;
            }
        }

        Beatmap* map = BeatmapManager::Beatmaps()[i+beatmap_list_offset];

        GraphicsManager::Graphics().LoadBeatmapPicTexture(texid, map->BaseDir() + "/" + map->BackgroundFilename());

        pSprite* bg = new pSprite(TX_BEATMAP_ENTRY_BG, 351, 91 + (i * 94), 609, 80, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.03f);

        bg->OnClick = OnMapSpriteClick;
        bg->Clickable = true;
        bg->Tag = (void *) (size_t) (i + beatmap_list_offset);

        mSpriteManager.Add(bg);

        pSprite* pic = new pSprite(texid, 400, 91 + (i * 94), 80, 80, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.04f);
        mSpriteManager.Add(pic);

        pText* mapTitle = new pText(map->Title(), FONT_CONSOLE, 494, 107 + (i * 94));
        mapTitle->Z = 0;

        pText* mapArtist = new pText(map->Artist(), FONT_CONSOLE, 494, 127 + (i * 94));
        mapArtist->Z = 0;

        pText* mapVersion = new pText(map->Version(), FONT_CONSOLE, 494, 147 + (i * 94));
        mapVersion->Z = 0;

        mSpriteManager.Add(mapTitle);
        mSpriteManager.Add(mapArtist);
        mSpriteManager.Add(mapVersion);
        mEntiesDisplayed++;
    }
}

void SongSelect::HandleInput()
{

}

void SongSelect::Update()
{
    if (shouldHandlePageUpdate) {
        shouldHandlePageUpdate = false;
        UpdateSonglist();
    }

	if (mDeltaX != 0)
	{
		uint32_t delta = mDeltaX / 6;
		
		if (delta == 0 && mDeltaX != 0)
			delta = MathHelper::Sgn(mDeltaX);
		
		mDeltaX -= delta;
		//MoveSongList(delta);
	}
	
	mSpriteManager.Draw();

	// beatmap names
}

void SongSelect::MoveSongList(int32_t dx)
{	
	
}

void SongSelect::MoveSongListLeft()
{

}

void SongSelect::MoveSongListRight()
{

}

