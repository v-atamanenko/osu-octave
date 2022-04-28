#pragma once

#include <fstream>
#include <limits>
#include <dirent.h>

#include "defines.h"
#include "types.h"

#include "DataStorage/Settings.h"
#include "Helpers/IO.h"

#include "JSON.hpp"

using json = nlohmann::json;

class I18n
{
    public:
        static std::string get(const std::string& key) {
            if (lang[key].is_null()) {
                fprintf(stderr, "String %s is not defined in the current language %s.\n", key.c_str(), Settings::get_str("language").c_str());
                return lang_default[key].get<std::string>();
            }
            return lang[key].get<std::string>();
        }

        static void clear() {
            lang_default = {
                    {"loading", "LOADING"},
                    {"loading_settings", "loading settings..."},
                    {"loading_bm_changes", "looking for changes in beatmaps..."},
                    {"loading_bm_no_changes", "beatmap index up to date! loading previews..."},
                    {"loading_bm_has_changes", "beatmaps changed. rebuilding index..."},
                    {"loading_bm_rebuilt", "beatmap index rebuit! loading previews..."},
                    {"loading_fonts", "loading fonts..."},
                    {"loading_done", "- tap to start -"},

                    {"score", "Score:"},
                    {"accuracy", "Accuracy:"},
                    {"personal_best", "Personal best:"},

                    {"btn_about", "About"},
                    {"btn_back", "Back"},
                    {"btn_back_touch", "Back touch"},
                    {"btn_disable", "Disable"},
                    {"btn_enable", "Enable"},
                    {"btn_front_touch", "Front touch"},
                    {"btn_quit", "Quit"},
                    {"btn_random", "Random"},
                    {"btn_resume", "Resume"},
                    {"btn_retry", "Retry"},
                    {"btn_settings", "Settings"},

                    {"settings_tab_general", "general"},
                    {"settings_tab_gameplay", "gameplay"},
                    {"settings_tab_sound", "sound"},
                    {"settings_volume_hitsounds", "Hit sounds"},
                    {"settings_volume_music", "Music"},
                    {"settings_volume_menumusic", "Menu music"},
                    {"settings_volume_uisounds", "UI sounds"},
                    {"settings_bgdim", "Background dim"},
                    {"settings_hoscale", "Objects scaling"},
                    {"settings_skin", "Skin"},
                    {"settings_nofail", "No-Fail"},
                    {"settings_vitaUseBackTouch", "Scheme"},
                    {"settings_controlScheme", "Action button"},
                    {"settings_enableStacking", "Stacking"},
                    {"settings_appearance", "Appearance"},
                    {"settings_system", "System"},
                    {"settings_language", "Language"},
                    {"settings_mods", "Mods"},
                    {"settings_controls", "Controls"},
                    {"settings_other", "Other"},
                    {"settings_volume", "Volume"},
                    {"settings_mods_notice", "Other mods are coming in the next versions..."},
                    {"settings_stacking_notice", "(Experimental)"},

                    {"filter_all", "ALL"},
                    {"filter_a_e", "A-E"},
                    {"filter_f_j", "F-J"},
                    {"filter_k_o", "K-O"},
                    {"filter_p_t", "P-T"},
                    {"filter_u_z", "U-Z"},
            };

            lang = lang_default;
        }

        /**
         * Get a vector of available languages in the languages directory.
         * @param vec Reference to a vector of strings to put the available languages into
         * @return Index of current (enabled) language in vec
         */
        static OOUInt get_available_langs(std::vector<std::string>& langs) {
            char path[PATH_MAX];
            snprintf(path, PATH_MAX, "%s%s",
                     DEF_DataDirectory, DEF_LanguagesSubdirectory);

            get_files_in_directory(path, langs, true);

            if (std::find(langs.begin(), langs.end(), "English") == langs.end()) {
                // If English is not present, add it anyway as it is the default.
                langs.emplace_back("English");
            }

            std::string current_lang = Settings::get_str("language");
            int ret = 0;
            for (const auto& a_lang : langs) {
                if (a_lang == current_lang) break;
                ret++;
            }
            if (ret >= langs.size()) {
                ret = langs.size() - 1;
            }

            return ret;
        }

        static void autodetect_language() {
            // TODO: detect preferred language automatically on various platforms
            Settings::set_str("language", "English");
        }

        static void load() {
            clear();

            if (Settings::get_str("language") == "default") {
                autodetect_language();
            }

            if (Settings::get_str("language") == "English") {
                char p[PATH_MAX];
                snprintf(p, PATH_MAX, "%s%sEnglish.json", DEF_DataDirectory, DEF_LanguagesSubdirectory);

                std::ofstream o(p);
                o << std::setw(4) << lang_default << std::endl;
                return;
            }

            char path[PATH_MAX];
            snprintf(path, PATH_MAX,
                     "%s%s%s.json",
                     DEF_DataDirectory,
                     DEF_LanguagesSubdirectory,
                     Settings::get_str("language").c_str());

            std::ifstream i(path);
            json j = json::parse(i, nullptr, false);

            if (j.is_discarded()) {
                fprintf(stderr, "JSON parse error, proceeding with default language.\n");
                return;
            }

            lang = j;
        }

    private:
        static json lang_default;
        static json lang;
};
