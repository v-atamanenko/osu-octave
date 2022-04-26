#include "Welcome.h"
#include "DataStorage/I18n.h"

void TapToStartHandler(pDrawable* self, OOInt x, OOInt y) {
    ChangeModeOnFrameEnd(MODE_SONGSELECT);
}

Welcome::Welcome() {
    GraphicsManager::Graphics().LoadTexturesForMode(MODE_WELCOME);

    mBG = new pSprite(TX_WELCOME_BG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255);
    mLogo = new Logo(480, 272, false);

    mStatus = new pText(I18n::get("loading_settings"), FONT_PIXEL, 480, 422, Skins::get_options().FontColor_LoadingScreen);
    mStatus->Origin = ORIGIN_CENTER;

    mSpriteManager.Add(mBG);
    mLogo->AddToSpriteManager(mSpriteManager);
    mSpriteManager.Add(mStatus);
}

void Welcome::HandleInput() {}

void Welcome::Redraw() {
    GraphicsManager::Clear();
    mSpriteManager.Draw(true);
    GraphicsManager::Present();
}

void Welcome::Update() {
    mSpriteManager.Draw();
    mLogo->Update();

    if (mStage == STAGE_LOAD_SETTINGS) {
        Welcome::Redraw();

        Scores::load();
        InputHelper::InitInput();
        AudioManager::Engine().Initialize();

        std::string music_path = std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + Settings::get_str("skin") + "/sounds/bgm.mp3";

        mStatus->Text = I18n::get("loading_bm_changes");
        mStage = STAGE_CHECK_INDEX;
        return;
    }

    if (mStage == STAGE_CHECK_INDEX) {
        Welcome::Redraw();

        Beatmaps::load();
        if (BeatmapManager::CheckIndex()) {
            mStatus->Text = I18n::get("loading_bm_no_changes");
            mStage = STAGE_START_PREVIEWBUFFER;
        } else {
            mStatus->Text = I18n::get("loading_bm_has_changes");
            mStage = STAGE_LOAD_INDEX;
        }
        return;
    }

    if (mStage == STAGE_LOAD_INDEX) {
        Welcome::Redraw();

        Beatmaps::clear();
        BeatmapManager::BuildCollection();
        mStatus->Text = I18n::get("loading_bm_rebuilt");
        mStage = STAGE_START_PREVIEWBUFFER;
        return;
    }

    if (mStage == STAGE_START_PREVIEWBUFFER) {
        Welcome::Redraw();

        BeatmapManager::Filter(Settings::get_beatmapfilter("activeFilter"));
        BeatmapManager::BuildMap();
        PreviewBuffer::GetInstance().Init();
        PreviewBuffer::GetInstance().lastAppliedFilter = Settings::get_beatmapfilter("activeFilter");
        mStatus->Text = I18n::get("loading_fonts");
        mStage = STAGE_LOAD_FONTS;
        return;
    }

    if (mStage == STAGE_LOAD_FONTS) {
        Welcome::Redraw();

        InputHelper::vitaUseBackTouch = false; // VITA: Disable back touch for menu.
        TextManager::InitDeferred();
        mBG->OnClick = TapToStartHandler;
        mBG->Clickable = true;
        mStatus->Text = I18n::get("loading_done");
        mStage = STAGE_DONE;

        AudioManager::PlayWelcome();
        mLogo->StartAnimation(1220+2142); // magic numbers are the lengths of two sound effects before bgm starts
    }
}
