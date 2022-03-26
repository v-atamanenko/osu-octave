#include "Welcome.h"

#include "Graphics/pText.h"
#include "Helpers/PreviewBuffer.h"
#include "DataStorage/Settings.h"
#include "DataStorage/Scores.h"

void TapToStartHandler(pDrawable* self, uint16_t x, uint16_t y) {
    ChangeModeOnFrameEnd(MODE_SONGSELECT);
}

Welcome::Welcome() {
    GraphicsManager::Graphics().LoadTexturesForMode(MODE_WELCOME);

    mBG = new pSprite(TX_WELCOME_BG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255);
    mLogo = new pSprite(TX_LOGO, 368, 210, 224, 123, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255);

    mStatus = new pText("loading settings...", FONT_PIXEL, 480, 422, SDL_Color());
    mStatus->Origin = ORIGIN_CENTER;

    mSpriteManager.Add(mBG);
    mSpriteManager.Add(mLogo);
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

    if (mStage == STAGE_LOAD_SETTINGS) {
        Welcome::Redraw();

        Scores::load();
        InputHelper::InitInput();

        std::string music_path = std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + Settings::get_str("skin") + "/sounds/bgm.mp3";
        AudioManager::Engine().MusicLoad(music_path);

        mStatus->Text = "looking for changes in beatmaps...";
        mStage = STAGE_CHECK_INDEX;
        return;
    }

    if (mStage == STAGE_CHECK_INDEX) {
        Welcome::Redraw();

        Beatmaps::load();
        if (BeatmapManager::CheckIndex()) {
            mStatus->Text = "beatmap index up to date! loading previews...";
            mStage = STAGE_START_PREVIEWBUFFER;
        } else {
            mStatus->Text = "beatmaps changed. rebuilding index...";
            mStage = STAGE_LOAD_INDEX;
        }
        return;
    }

    if (mStage == STAGE_LOAD_INDEX) {
        Welcome::Redraw();

        Beatmaps::clear();
        BeatmapManager::BuildCollection();
        mStatus->Text = "beatmap index rebuit! loading previews...";
        mStage = STAGE_START_PREVIEWBUFFER;
        return;
    }

    if (mStage == STAGE_START_PREVIEWBUFFER) {
        Welcome::Redraw();

        BeatmapManager::Filter(Settings::get_beatmapfilter("activeFilter"));
        BeatmapManager::BuildMap();
        PreviewBuffer::GetInstance().Init();
        PreviewBuffer::GetInstance().lastAppliedFilter = Settings::get_beatmapfilter("activeFilter");
        mStatus->Text = "loading fonts...";
        mStage = STAGE_LOAD_FONTS;
        return;
    }

    if (mStage == STAGE_LOAD_FONTS) {
        Welcome::Redraw();

        InputHelper::vitaUseBackTouch = false; // VITA: Disable back touch for menu.
        TextManager::InitDeferred();
        mBG->OnClick = TapToStartHandler;
        mBG->Clickable = true;
        mStatus->Text = "- tap to start -";
        mStage = STAGE_DONE;
        AudioManager::Engine().MusicPlay();
    }
}
