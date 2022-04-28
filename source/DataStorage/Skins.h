#pragma once

#include <fstream>
#include <limits>

#include "../lib/EnumIterator.h"
#include "Helpers/IO.h"
#include "JSON.hpp"

#include "DataStorage/Settings.h"

#include "types.h"
#include "defines.h"

#include "Graphics/GfxInfo.h"
#include "Helpers/AudioManager.h"

typedef Iterator<TextureType, TextureType::TX_PLAY_CIRCLE, TextureType::TX_ABOUT> texTypeIterator;
typedef Iterator<HitSoundName, HitSoundName::HITSOUND_NORMAL_HITNORMAL, HitSoundName::HITSOUND_SOFT_SPINNERBONUS> hitSoundsIterator;
typedef Iterator<UISoundName, UISoundName::UISOUND_APPLAUSE, UISoundName::UISOUND_WELCOME_PIANO> uiSoundsIterator;
typedef Iterator<FontName, FontName::FONT_PRIMARY, FontName::FONT_NUMBERING> fontNamesIterator;

using json = nlohmann::json;

typedef struct Skin {
    std::string Name;
    std::string Author; // Not used
    std::string Version; // Not used

    bool TintHitCircles = true;
    bool TintSliderCircles = true;
    bool TintSliderPath = true;
    bool TintSliderBorder = false;
    bool TintSliderBall = false;
    bool TintUseBeatmapColors = true;

    SDL_Color TintCustomColor1 = {0,0,0,0};
    SDL_Color TintCustomColor2 = {0,0,0,0};
    SDL_Color TintCustomColor3 = {0,0,0,0};
    SDL_Color TintCustomColor4 = {0,0,0,0};

    SDL_Color FontColor_LoadingScreen = {67,19,115,255};
    SDL_Color FontColor_SortingButton = {67,19,115,255};
    SDL_Color FontColor_SortingButtonActive = {255,255,255,255};
    SDL_Color FontColor_BeatmapDescription = {67,19,115,255};
    SDL_Color FontColor_Pagination = {67,19,115,255};
    SDL_Color FontColor_MenuButton = {67,19,115,255};
    SDL_Color FontColor_MenuButtonActive = {255,255,255,255};
    SDL_Color FontColor_SettingsLabel = {67,19,115,255};
    SDL_Color FontColor_SettingsLabelActive = {255,255,255,255};
    SDL_Color FontColor_EndGameScore = {67,19,115,255};

    bool ShowSliderEndCircle = true;
} Skin;

inline SDL_Color arr_to_clr(const json& j, const std::string& obj) {
    std::array<int, 4> clrs = {};
    j.at(obj).get_to(clrs);
    return {clrs[0],clrs[1],clrs[2],clrs[3]};
}

inline void to_json(json& j, const Skin s) {
    j = json{
        {"Name", s.Name},
        {"Author", s.Author},
        {"Version", s.Version},

        {"TintHitCircles", s.TintHitCircles},
        {"TintSliderCircles", s.TintSliderCircles},
        {"TintSliderPath", s.TintSliderCircles},
        {"TintSliderBorder", s.TintSliderBorder},
        {"TintSliderBall", s.TintSliderBall},
        {"TintUseBeatmapColors", s.TintUseBeatmapColors},

        {"TintCustomColor1", {s.TintCustomColor1.r, s.TintCustomColor1.g, s.TintCustomColor1.b, s.TintCustomColor1.a}},
        {"TintCustomColor2", {s.TintCustomColor2.r, s.TintCustomColor2.g, s.TintCustomColor2.b, s.TintCustomColor2.a}},
        {"TintCustomColor3", {s.TintCustomColor3.r, s.TintCustomColor3.g, s.TintCustomColor3.b, s.TintCustomColor3.a}},
        {"TintCustomColor4", {s.TintCustomColor4.r, s.TintCustomColor4.g, s.TintCustomColor4.b, s.TintCustomColor4.a}},

        {"FontColor_LoadingScreen", {s.FontColor_LoadingScreen.r, s.FontColor_LoadingScreen.g, s.FontColor_LoadingScreen.b, s.FontColor_LoadingScreen.a}},
        {"FontColor_SortingButton", {s.FontColor_SortingButton.r, s.FontColor_SortingButton.g, s.FontColor_SortingButton.b, s.FontColor_SortingButton.a}},
        {"FontColor_SortingButtonActive", {s.FontColor_SortingButtonActive.r, s.FontColor_SortingButtonActive.g, s.FontColor_SortingButtonActive.b, s.FontColor_SortingButtonActive.a}},
        {"FontColor_BeatmapDescription", {s.FontColor_BeatmapDescription.r, s.FontColor_BeatmapDescription.g, s.FontColor_BeatmapDescription.b, s.FontColor_BeatmapDescription.a}},
        {"FontColor_Pagination", {s.FontColor_Pagination.r, s.FontColor_Pagination.g, s.FontColor_Pagination.b, s.FontColor_Pagination.a}},
        {"FontColor_MenuButton", {s.FontColor_MenuButton.r, s.FontColor_MenuButton.g, s.FontColor_MenuButton.b, s.FontColor_MenuButton.a}},
        {"FontColor_MenuButtonActive", {s.FontColor_MenuButtonActive.r, s.FontColor_MenuButtonActive.g, s.FontColor_MenuButtonActive.b, s.FontColor_MenuButtonActive.a}},
        {"FontColor_SettingsLabel", {s.FontColor_SettingsLabel.r, s.FontColor_SettingsLabel.g, s.FontColor_SettingsLabel.b, s.FontColor_SettingsLabel.a}},
        {"FontColor_SettingsLabelActive", {s.FontColor_SettingsLabelActive.r, s.FontColor_SettingsLabelActive.g, s.FontColor_SettingsLabelActive.b, s.FontColor_SettingsLabelActive.a}},
        {"FontColor_EndGameScore", {s.FontColor_EndGameScore.r, s.FontColor_EndGameScore.g, s.FontColor_EndGameScore.b, s.FontColor_EndGameScore.a}},

        {"ShowSliderEndCircle", s.ShowSliderEndCircle}
    };
}

inline void from_json(const json& j, Skin& s) {
    j.at("Name").get_to(s.Name);
    j.at("Author").get_to(s.Author);
    j.at("Version").get_to(s.Version);

    j.at("TintHitCircles").get_to(s.TintHitCircles);
    j.at("TintSliderCircles").get_to(s.TintSliderCircles);
    j.at("TintSliderPath").get_to(s.TintSliderPath);
    j.at("TintSliderBorder").get_to(s.TintSliderBorder);
    j.at("TintSliderBall").get_to(s.TintSliderBall);
    j.at("TintUseBeatmapColors").get_to(s.TintUseBeatmapColors);

    s.TintCustomColor1 = arr_to_clr(j, "TintCustomColor1");
    s.TintCustomColor2 = arr_to_clr(j, "TintCustomColor2");
    s.TintCustomColor3 = arr_to_clr(j, "TintCustomColor3");
    s.TintCustomColor4 = arr_to_clr(j, "TintCustomColor4");

    s.FontColor_LoadingScreen = arr_to_clr(j, "FontColor_LoadingScreen");
    s.FontColor_SortingButton = arr_to_clr(j, "FontColor_SortingButton");
    s.FontColor_SortingButtonActive = arr_to_clr(j, "FontColor_SortingButtonActive");
    s.FontColor_BeatmapDescription = arr_to_clr(j, "FontColor_BeatmapDescription");
    s.FontColor_Pagination = arr_to_clr(j, "FontColor_Pagination");
    s.FontColor_MenuButton = arr_to_clr(j, "FontColor_MenuButton");
    s.FontColor_MenuButtonActive = arr_to_clr(j, "FontColor_MenuButtonActive");
    s.FontColor_SettingsLabel = arr_to_clr(j, "FontColor_SettingsLabel");
    s.FontColor_SettingsLabelActive = arr_to_clr(j, "FontColor_SettingsLabelActive");
    s.FontColor_EndGameScore = arr_to_clr(j, "FontColor_EndGameScore");

    j.at("ShowSliderEndCircle").get_to(s.ShowSliderEndCircle);
}

class Skins
{
    public:
        /**
         * Get a vector of available skins in the skins directory.
         * @param vec Reference to a vector of strings to put the available skins into
         * @return Index of current (enabled) skin in vec
         */
        static int get_available_skins(std::vector<std::string>& vec) {
            std::string current_skin = Settings::get_str("skin");
            int ret = 0;

            char path[PATH_MAX];
            snprintf(path, PATH_MAX, "%s%s", DEF_DataDirectory, DEF_SkinsSubdirectory);

            DIR *dir = opendir(path);

            struct dirent *entry = readdir(dir);

            int i = 0;
            while (entry != nullptr) {
                char entry_path[PATH_MAX];
                snprintf(entry_path, PATH_MAX, "%s%s", path, entry->d_name);
                if (is_dir(entry_path)) {
                    std::string dir_name = entry->d_name;
                    if (dir_name != "." && dir_name != "..") {
                        vec.emplace_back(dir_name);
                        if (dir_name == current_skin) {
                            ret = i;
                        }
                        i++;
                    }
                }

                entry = readdir(dir);
            }

            closedir(dir);
            return ret;
        }

        static void init_skin() {
            load_skin_options();
            index_gameplay_sounds();
            index_ui_sounds();
            index_textures();
            index_fonts();

            loaded = true;
        }

        static std::string get_res_path(HitSoundName hsn) {
            assert(loaded);
            return gameplay_sounds.at(hsn);
        }

        static std::string get_res_path(UISoundName usn) {
            assert(loaded);
            return ui_sounds.at(usn);
        }

        static std::string get_res_path(TextureType tt) {
            assert(loaded);
            return textures.at(tt);
        }

        static std::string get_res_path(FontName fn) {
            assert(loaded);
            return fonts.at(fn);
        }

        static Skin& get_options() {
            return skin_options;
        }

    private:
        static json skins;
        static bool loaded;

        static Skin skin_options;
        static std::map<HitSoundName, std::string> gameplay_sounds;
        static std::map<UISoundName, std::string> ui_sounds;
        static std::map<TextureType, std::string> textures;
        static std::map<FontName, std::string> fonts;

        static void load_skin_options() {
            json skin_options_j = {};

            char path[PATH_MAX];
            snprintf(path, PATH_MAX, "%s%s%s%s", DEF_DataDirectory, DEF_SkinsSubdirectory, Settings::get_str("skin").c_str(), "/skin.json");

            std::ifstream i(path);
            json j = json::parse(i, nullptr, false);

            if (j.is_discarded()) {
                fprintf(stderr, "Skin.json JSON parse error, proceeding with empty. Path: %s\n", path);
                return;
            }

            skin_options = j.get<Skin>();
        }


        static void index_gameplay_sounds() {
            gameplay_sounds.clear();

            std::string path = std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + Settings::get_str("skin") + "/sounds/";
            std::string path_default = std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + "default" + "/sounds/";

            for (HitSoundName hsn : hitSoundsIterator()) {
                std::string filename;

                switch (hsn) {
                    case HITSOUND_NORMAL_HITNORMAL:
                        filename = "normal-hitnormal.wav";
                        break;
                    case HITSOUND_NORMAL_HITWHISTLE:
                        filename = "normal-hitwhistle.wav";
                        break;
                    case HITSOUND_NORMAL_HITFINISH:
                        filename = "normal-hitfinish.wav";
                        break;
                    case HITSOUND_NORMAL_HITCLAP:
                        filename = "normal-hitclap.wav";
                        break;
                    case HITSOUND_NORMAL_SLIDERTICK:
                        filename = "normal-slidertick.wav";
                        break;
                    case HITSOUND_NORMAL_SLIDERSLIDE:
                        filename = "normal-sliderslide.wav";
                        break;
                    case HITSOUND_NORMAL_SLIDERWHISTLE:
                        filename = "normal-sliderwhistle.wav";
                        break;
                    case HITSOUND_NORMAL_SPINNERSPIN:
                        filename = "spinnerspin.wav";
                        break;
                    case HITSOUND_NORMAL_SPINNERBONUS:
                        filename = "spinnerbonus.wav";
                        break;
                    case HITSOUND_SOFT_HITNORMAL:
                        filename = "soft-hitnormal.wav";
                        break;
                    case HITSOUND_SOFT_HITWHISTLE:
                        filename = "soft-hitwhistle.wav";
                        break;
                    case HITSOUND_SOFT_HITFINISH:
                        filename = "soft-hitfinish.wav";
                        break;
                    case HITSOUND_SOFT_HITCLAP:
                        filename = "soft-hitclap.wav";
                        break;
                    case HITSOUND_SOFT_SLIDERTICK:
                        filename = "soft-slidertick.wav";
                        break;
                    case HITSOUND_SOFT_SLIDERSLIDE:
                        filename = "soft-sliderslide.wav";
                        break;
                    case HITSOUND_SOFT_SLIDERWHISTLE:
                        filename = "soft-sliderwhistle.wav";
                        break;
                    case HITSOUND_SOFT_SPINNERSPIN:
                        filename = "spinnerspin.wav";
                        break;
                    case HITSOUND_SOFT_SPINNERBONUS:
                        filename = "spinnerbonus.wav";
                        break;
                }

                if (file_exists(path + filename)) {
                    gameplay_sounds[hsn] = path + filename;
                } else {
                    gameplay_sounds[hsn] = path_default + filename;
                }
            }
        }

        static void index_ui_sounds() {
            ui_sounds.clear();

            std::string path = std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + Settings::get_str("skin") + "/ui-sounds/";
            std::string path_default = std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + "default" + "/ui-sounds/";

            for (UISoundName usn : uiSoundsIterator()) {
                std::string filename;

                switch (usn) {
                    case UISOUND_APPLAUSE:
                        filename = "applause.wav";
                        break;
                    case UISOUND_BGM:
                        filename = "bgm.mp3";
                        break;
                    case UISOUND_CHECK_OFF:
                        filename = "check-off.wav";
                        break;
                    case UISOUND_CHECK_ON:
                        filename = "check-on.wav";
                        break;
                    case UISOUND_CLICK_CLOSE:
                        filename = "click-close.wav";
                        break;
                    case UISOUND_CLICK_SHORT_CONFIRM:
                        filename = "click-short-confirm.wav";
                        break;
                    case UISOUND_COMBOBREAK:
                        filename = "combobreak.wav";
                        break;
                    case UISOUND_FAILSOUND:
                        filename = "failsound.wav";
                        break;
                    case UISOUND_MENUBACK:
                        filename = "menuback.wav";
                        break;
                    case UISOUND_MENUCLICK:
                        filename = "menuclick.wav";
                        break;
                    case UISOUND_MENUHIT:
                        filename = "menuhit.wav";
                        break;
                    case UISOUND_SEEYA:
                        filename = "seeya.wav";
                        break;
                    case UISOUND_WELCOME:
                        filename = "welcome.wav";
                        break;
                    case UISOUND_WELCOME_PIANO:
                        filename = "welcome_piano.wav";
                        break;
                }

                if (file_exists(path + filename)) {
                    ui_sounds[usn] = path + filename;
                } else {
                    ui_sounds[usn] = path_default + filename;
                }
            }
        }

        static void index_textures() {
            textures.clear();

            std::string path = std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + Settings::get_str("skin") + "/";
            std::string path_default = std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + "default" + "/";

            for (TextureType tt : texTypeIterator()) {
                std::string filename;
                std::string subfolder;

                switch (tt) {
                    case TX_ABOUT:
                        subfolder = "ui/";
                        filename = "about.png";
                        break;
                    case TX_BUTTON_ARROW:
                        subfolder = "ui/";
                        filename = "button-arrow.png";
                        break;
                    case TX_BUTTON_BIG:
                        subfolder = "ui/";
                        filename = "button-big.png";
                        break;
                    case TX_BUTTON_BIG_ACTIVE:
                        subfolder = "ui/";
                        filename = "button-big-active.png";
                        break;
                    case TX_BUTTON_MED:
                        subfolder = "ui/";
                        filename = "button-med.png";
                        break;
                    case TX_BUTTON_PLAY:
                        subfolder = "ui/";
                        filename = "button-play.png";
                        break;
                    case TX_BUTTON_RADIO:
                        subfolder = "ui/";
                        filename = "button-radio.png";
                        break;
                    case TX_BUTTON_SETTINGS_ARROW:
                        subfolder = "ui/";
                        filename = "button-settings-arrow.png";
                        break;
                    case TX_BUTTON_SETTINGS_ARROW_ACTIVE:
                        subfolder = "ui/";
                        filename = "button-settings-arrow-active.png";
                        break;
                    case TX_BUTTON_SLIDER_CIRCLE:
                        subfolder = "ui/";
                        filename = "slider-circle.png";
                        break;
                    case TX_BUTTON_SLIDER_CIRCLE_ACTIVE:
                        subfolder = "ui/";
                        filename = "slider-circle-active.png";
                        break;
                    case TX_BUTTON_SM:
                        subfolder = "ui/";
                        filename = "button-sm.png";
                        break;
                    case TX_BUTTON_SM_ACTIVE:
                        subfolder = "ui/";
                        filename = "button-sm-active.png";
                        break;
                    case TX_BUTTON_XS:
                        subfolder = "ui/";
                        filename = "button-xs.png";
                        break;
                    case TX_BUTTON_XS_ACTIVE:
                        subfolder = "ui/";
                        filename = "button-xs-active.png";
                        break;
                    case TX_GAMEOVER_BG:
                        subfolder = "ui/";
                        filename = "gameover-bg.png";
                        break;
                    case TX_LOGO:
                        subfolder = "ui/";
                        filename = "logo.png";
                        break;
                    case TX_MENU_BG:
                        subfolder = "ui/";
                        filename = "menu-bg.png";
                        break;
                    case TX_PAUSE_BG:
                        subfolder = "ui/";
                        filename = "pause-bg.png";
                        break;
                    case TX_PLAY_CIRCLE:
                        subfolder = "gameplay/";
                        filename = "circle.png";
                        break;
                    case TX_PLAY_CIRCLEAPPROACH:
                        subfolder = "gameplay/";
                        filename = "circleapproach.png";
                        break;
                    case TX_PLAY_CIRCLEOVERLAY:
                        subfolder = "gameplay/";
                        filename = "circleoverlay.png";
                        break;
                    case TX_PLAY_CURSOR:
                        subfolder = "ui/";
                        filename = "cursor.png";
                        break;
                    case TX_PLAY_HIT0:
                        subfolder = "gameplay/";
                        filename = "hit0.png";
                        break;
                    case TX_PLAY_HIT100:
                        subfolder = "gameplay/";
                        filename = "hit100.png";
                        break;
                    case TX_PLAY_HIT100K:
                        subfolder = "gameplay/";
                        filename = "hit100k.png";
                        break;
                    case TX_PLAY_HIT300:
                        subfolder = "gameplay/";
                        filename = "hit300.png";
                        break;
                    case TX_PLAY_HIT300G:
                        subfolder = "gameplay/";
                        filename = "hit300g.png";
                        break;
                    case TX_PLAY_HIT300K:
                        subfolder = "gameplay/";
                        filename = "hit300k.png";
                        break;
                    case TX_PLAY_HIT50:
                        subfolder = "gameplay/";
                        filename = "hit50.png";
                        break;
                    case TX_PLAY_NUMBER_0:
                        subfolder = "gameplay/";
                        filename = "default-0.png";
                        break;
                    case TX_PLAY_NUMBER_1:
                        subfolder = "gameplay/";
                        filename = "default-1.png";
                        break;
                    case TX_PLAY_NUMBER_2:
                        subfolder = "gameplay/";
                        filename = "default-2.png";
                        break;
                    case TX_PLAY_NUMBER_3:
                        subfolder = "gameplay/";
                        filename = "default-3.png";
                        break;
                    case TX_PLAY_NUMBER_4:
                        subfolder = "gameplay/";
                        filename = "default-4.png";
                        break;
                    case TX_PLAY_NUMBER_5:
                        subfolder = "gameplay/";
                        filename = "default-5.png";
                        break;
                    case TX_PLAY_NUMBER_6:
                        subfolder = "gameplay/";
                        filename = "default-6.png";
                        break;
                    case TX_PLAY_NUMBER_7:
                        subfolder = "gameplay/";
                        filename = "default-7.png";
                        break;
                    case TX_PLAY_NUMBER_8:
                        subfolder = "gameplay/";
                        filename = "default-8.png";
                        break;
                    case TX_PLAY_NUMBER_9:
                        subfolder = "gameplay/";
                        filename = "default-9.png";
                        break;
                    case TX_PLAY_SCOREBAR:
                        subfolder = "ui/";
                        filename = "scorebar.png";
                        break;
                    case TX_PLAY_SCOREBAR_BAR:
                        subfolder = "ui/";
                        filename = "scorebar_colour.png";
                        break;
                    case TX_PLAY_SCOREBAR_KI:
                        subfolder = "ui/";
                        filename = "scorebar_ki.png";
                        break;
                    case TX_PLAY_SCOREBAR_KIDANGER:
                        subfolder = "ui/";
                        filename = "scorebar_kidanger.png";
                        break;
                    case TX_PLAY_SCOREBAR_KIDANGER2:
                        subfolder = "ui/";
                        filename = "scorebar_kidanger2.png";
                        break;
                    case TX_PLAY_SLIDER10:
                        subfolder = "gameplay/";
                        filename = "slider10.png";
                        break;
                    case TX_PLAY_SLIDER30:
                        subfolder = "gameplay/";
                        filename = "slider30.png";
                        break;
                    case TX_PLAY_SLIDERB0:
                        subfolder = "gameplay/";
                        filename = "sliderb0.png";
                        break;
                    case TX_PLAY_SLIDERB1:
                        subfolder = "gameplay/";
                        filename = "sliderb1.png";
                        break;
                    case TX_PLAY_SLIDERB2:
                        subfolder = "gameplay/";
                        filename = "sliderb2.png";
                        break;
                    case TX_PLAY_SLIDERB3:
                        subfolder = "gameplay/";
                        filename = "sliderb3.png";
                        break;
                    case TX_PLAY_SLIDERB4:
                        subfolder = "gameplay/";
                        filename = "sliderb4.png";
                        break;
                    case TX_PLAY_SLIDERB5:
                        subfolder = "gameplay/";
                        filename = "sliderb5.png";
                        break;
                    case TX_PLAY_SLIDERB6:
                        subfolder = "gameplay/";
                        filename = "sliderb6.png";
                        break;
                    case TX_PLAY_SLIDERB7:
                        subfolder = "gameplay/";
                        filename = "sliderb7.png";
                        break;
                    case TX_PLAY_SLIDERB8:
                        subfolder = "gameplay/";
                        filename = "sliderb8.png";
                        break;
                    case TX_PLAY_SLIDERB9:
                        subfolder = "gameplay/";
                        filename = "sliderb9.png";
                        break;
                    case TX_PLAY_SLIDERBODY:
                        subfolder = "gameplay/";
                        filename = "sliderbody.png";
                        break;
                    case TX_PLAY_SLIDERBORDER:
                        subfolder = "gameplay/";
                        filename = "sliderborder.png";
                        break;
                    case TX_PLAY_SLIDERFOLLOW:
                        subfolder = "gameplay/";
                        filename = "sliderfollow.png";
                        break;
                    case TX_PLAY_SLIDERREVERSE:
                        subfolder = "gameplay/";
                        filename = "sliderreverse.png";
                        break;
                    case TX_PLAY_SLIDERTICK:
                        subfolder = "gameplay/";
                        filename = "slidertick.png";
                        break;
                    case TX_PLAY_SPINNER:
                        subfolder = "gameplay/";
                        filename = "spinner.png";
                        break;
                    case TX_PLAY_SPINNERBARS:
                        subfolder = "gameplay/";
                        filename = "spinnerbars.png";
                        break;
                    case TX_PLAY_SPINNERBG:
                        subfolder = "gameplay/";
                        filename = "spinnerbg.png";
                        break;
                    case TX_PLAY_SPINNER_RPM_BG:
                        subfolder = "ui/";
                        filename = "spinner-rpm-bg.png";
                        break;
                    case TX_RANKING_A_LG:
                        subfolder = "ui/";
                        filename = "ranking-a-lg.png";
                        break;
                    case TX_RANKING_A_SM:
                        subfolder = "ui/";
                        filename = "ranking-a-sm.png";
                        break;
                    case TX_RANKING_B_LG:
                        subfolder = "ui/";
                        filename = "ranking-b-lg.png";
                        break;
                    case TX_RANKING_B_SM:
                        subfolder = "ui/";
                        filename = "ranking-b-sm.png";
                        break;
                    case TX_RANKING_C_LG:
                        subfolder = "ui/";
                        filename = "ranking-c-lg.png";
                        break;
                    case TX_RANKING_C_SM:
                        subfolder = "ui/";
                        filename = "ranking-c-sm.png";
                        break;
                    case TX_RANKING_D_LG:
                        subfolder = "ui/";
                        filename = "ranking-d-lg.png";
                        break;
                    case TX_RANKING_D_SM:
                        subfolder = "ui/";
                        filename = "ranking-d-sm.png";
                        break;
                    case TX_RANKING_F_LG:
                        subfolder = "ui/";
                        filename = "ranking-f-lg.png";
                        break;
                    case TX_RANKING_SS_LG:
                        subfolder = "ui/";
                        filename = "ranking-ss-lg.png";
                        break;
                    case TX_RANKING_SS_SM:
                        subfolder = "ui/";
                        filename = "ranking-ss-sm.png";
                        break;
                    case TX_RANKING_S_LG:
                        subfolder = "ui/";
                        filename = "ranking-s-lg.png";
                        break;
                    case TX_RANKING_S_SM:
                        subfolder = "ui/";
                        filename = "ranking-s-sm.png";
                        break;
                    case TX_RANKING_UNK_SM:
                        subfolder = "ui/";
                        filename = "ranking-unk-sm.png";
                        break;
                    case TX_SETTINGS_CONTROL_SELECTOR_1:
                        subfolder = "ui/";
                        filename = "controls-selector-1.png";
                        break;
                    case TX_SETTINGS_CONTROL_SELECTOR_2:
                        subfolder = "ui/";
                        filename = "controls-selector-2.png";
                        break;
                    case TX_SETTINGS_CONTROL_SELECTOR_3:
                        subfolder = "ui/";
                        filename = "controls-selector-3.png";
                        break;
                    case TX_SETTINGS_PANEL_GAMEPLAY:
                        subfolder = "ui/";
                        filename = "settings-panel-gameplay.png";
                        break;
                    case TX_SETTINGS_PANEL_GENERAL:
                        subfolder = "ui/";
                        filename = "settings-panel-general.png";
                        break;
                    case TX_STARS:
                        subfolder = "ui/";
                        filename = "stars.png";
                        break;
                    case TX_STARS_FILL:
                        subfolder = "ui/";
                        filename = "stars-fill.png";
                        break;
                    case TX_WELCOME_BG:
                        subfolder = "ui/";
                        filename = "welcome.png";
                        break;
                }

                if (file_exists(path + subfolder + filename)) {
                    textures[tt] = path + subfolder + filename;
                } else {
                    textures[tt] = path_default + subfolder + filename;
                }
            }
        }

        static void index_fonts() {
            fonts.clear();

            std::string path =
                    std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + Settings::get_str("skin") +
                    "/fonts/";
            std::string path_default =
                    std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + "default" + "/fonts/";

            for (FontName fn: fontNamesIterator()) {
                std::string filename;

                switch (fn) {
                    case FONT_NUMBERING:
                        filename = "numbering.ttf";
                        break;
                    case FONT_PRIMARY:
                    case FONT_PRIMARY_SMALL:
                    case FONT_PRIMARY_SMALLER:
                    case FONT_PRIMARY_BIG:
                        filename = "primary.ttf";
                        break;
                    case FONT_PRIMARY_BOLD:
                    case FONT_PRIMARY_SMALL_BOLD:
                    case FONT_PRIMARY_SMALLER_BOLD:
                        filename = "primary-bold.ttf";
                        break;
                    case FONT_SCORE:
                        if (file_exists(path + "score.ttf")) {
                            filename = "score.ttf";
                        } else {
                            filename = "primary.ttf";
                        }
                        break;
                    default:
                        filename = "primary.ttf";
                        break;
                }

                if (file_exists(path + filename)) {
                    fonts[fn] = path + filename;
                } else {
                    fonts[fn] = path_default + filename;
                }
            }
        }
};
