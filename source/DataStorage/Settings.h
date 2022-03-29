#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <fstream>
#include <limits>
#include <dirent.h>

#include "defines.h"
#include "types.h"

#include "Helpers/JSON.hpp"
using json = nlohmann::json;

inline bool is_dir(char* p) {
    DIR* filetest = opendir(p);
    if (filetest == nullptr) {
        return false;
    } else {
        closedir(filetest);
        return true;
    }
}

class Settings
{
    public:
        static std::string get_str(const std::string& key) {
            if (settings[key].is_null()) {
                fprintf(stderr, "Setting %s is null. Resetting settings.\n", key.c_str());
                clear();
                return get_str(key);
            }
            return settings[key].get<std::string>();
        }
        static int get_int(const std::string& key) {
            if (settings[key].is_null()) {
                fprintf(stderr, "Setting %s is null. Resetting settings.\n", key.c_str());
                clear();
                return get_int(key);
            }
            return settings[key].get<int>();
        }
        static BeatmapFilter get_beatmapfilter(const std::string& key) {
            if (settings[key].is_null()) {
                fprintf(stderr, "Setting %s is null. Resetting settings.\n", key.c_str());
                clear();
                return get_beatmapfilter(key);
            }
            return settings[key].get<BeatmapFilter>();
        }
        static float get_float(const std::string& key) {
            if (settings[key].is_null()) {
                fprintf(stderr, "Setting %s is null. Resetting settings.\n", key.c_str());
                clear();
                return get_float(key);
            }
            return settings[key].get<float>();
        }
        static bool get_bool(const std::string& key) {
            if (settings[key].is_null()) {
                fprintf(stderr, "Setting %s is null. Resetting settings.\n", key.c_str());
                clear();
                return get_bool(key);
            }
            return settings[key].get<bool>();
        }
        static void get_controls(std::map<Control, std::vector<RawKey>>& controls, bool* vitaUseBackTouch) {
            controls.clear();
            if (!settings["controls"].is_array()) {
                fprintf(stderr, "Setting 'controls' is null. Resetting settings.\n");
                clear();
                return get_controls(controls, vitaUseBackTouch);
            }

            *vitaUseBackTouch = get_bool("vitaUseBackTouch");

            for (auto & it_1 : settings["controls"]) {
                // Now it_1 is a single [Control, std::vector<RawKey>] pair
                Control c = it_1[0].get<Control>();
                std::vector<RawKey> ctrls;

                for (auto & it_2 : it_1[1]) {
                    // Now it_2 is a single [int, KeyType] (aka RawKey) pair
                    int key = it_2[0].get<int>();
                    KeyType type = it_2[1].get<KeyType>();
                    ctrls.emplace_back(RawKey({key, type}));
                }

                controls.insert({c, ctrls});
            }
        }

        static int get_available_skins(std::vector<std::string>& vec) {
            std::string current_skin = get_str("skin");
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

        static void set_str(const std::string& key, const std::string& value) { settings[key] = value; }
        static void set_int(const std::string& key, const int value) { settings[key] = value; }
        static void set_beatmapfilter(const std::string& key, const BeatmapFilter value) { settings[key] = value; }
        static void set_float(const std::string& key, const float value) { settings[key] = value; }
        static void set_bool(const std::string& key, const bool value) { settings[key] = value; }

        static void save() {
            char path[PATH_MAX];
            snprintf(path, PATH_MAX, "%s%s", DEF_DataDirectory, DEF_SettingsFilename);

            std::ofstream o(path);
            o << std::setw(4) << settings << std::endl;
        }

        static void clear() {
            settings = {
                    {"page", 0},
                    {"noFail", true},
                    {"activeFilter", FILTER_NONE},
                    {"skin", "default"},
                    {"vitaUseBackTouch", false},
                    {"volume_hitsounds", 65.0},
                    {"volume_music", 70.0},
                    {"volume_menumusic", 40.0},
                    {"volume_uisounds", 65.0},
                    {"bgdim", 65.0},
                    {"hoscale", 25.0},
                    {"controls", {
                        {IH_CONTROL_ACTION, {{SDL_BUTTON_LEFT, IH_KEY_MOUSE}, {SDLK_z, IH_KEY_KEYBOARD}, {SDLK_x, IH_KEY_KEYBOARD}, {SDL_CONTROLLER_BUTTON_LEFTSHOULDER, IH_KEY_CONTROLLER}, {SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, IH_KEY_CONTROLLER}}},
                        {IH_CONTROL_SKIP, {{SDLK_SPACE, IH_KEY_KEYBOARD}, {SDL_CONTROLLER_BUTTON_A, IH_KEY_CONTROLLER}}},
                        {IH_CONTROL_QUIT, {{SDLK_ESCAPE, IH_KEY_KEYBOARD}, {SDL_CONTROLLER_BUTTON_START, IH_KEY_CONTROLLER}}}
                    }}
            };
        }

        static void load() {
            clear();

            char path[PATH_MAX];
            snprintf(path, PATH_MAX, "%s%s", DEF_DataDirectory, DEF_SettingsFilename);

            std::ifstream i(path);
            json j = json::parse(i, nullptr, false);

            if (j.is_discarded()) {
                fprintf(stderr, "JSON parse error, proceeding with default settings.\n");
                return;
            }

            settings = j;
        }

    private:
        static json settings;
};

#endif //_SETTINGS_H
