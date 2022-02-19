#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <fstream>
#include <limits>

#include "defines.h"

#include "Helpers/JSON.hpp"
using json = nlohmann::json;

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

class Settings
{
    public:
        static std::string get_str(const std::string& key) { return settings[key].get<std::string>(); }
        static int get_int(const std::string& key) { return settings[key].get<int>(); }
        static float get_float(const std::string& key) { return settings[key].get<float>(); }

        static void set_str(const std::string& key, const std::string& value) { settings[key] = value; }
        static void set_int(const std::string& key, const int value) { settings[key] = value; }
        static void set_float(const std::string& key, const float value) { settings[key] = value; }

        static void save() {
            char path[PATH_MAX];
            snprintf(path, PATH_MAX, "%s%s", DATA_DIR, SETS_FIL);

            std::ofstream o(path);
            o << std::setw(4) << settings << std::endl;
        }

        static void load() {
            settings = {
                {"page", 0}
            };

            char path[PATH_MAX];
            snprintf(path, PATH_MAX, "%s%s", DATA_DIR, SETS_FIL);

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
