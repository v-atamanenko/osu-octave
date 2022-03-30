#pragma once

#include <fstream>
#include <limits>

#include "defines.h"
#include "GameplayElements/Score.h"

#include "Helpers/JSON.hpp"

using json = nlohmann::json;

typedef struct Highscore {
    std::string checksum;
    OOUInt score = 0;
    OOUInt combo = 0;
    OOFloat accuracy = 0;
    char grade[GRADE_LEN] = {'U','N','\0'};
} Highscore;

inline void to_json(json& j, const Highscore h) {
    j = json{{"checksum", h.checksum}, {"score", h.score}, {"combo", h.combo},
             {"accuracy", h.accuracy}, {"grade", h.grade}};
}

inline void from_json(const json& j, Highscore& h) {
    j.at("checksum").get_to(h.checksum);
    j.at("score").get_to(h.score);
    j.at("combo").get_to(h.combo);
    j.at("accuracy").get_to(h.accuracy);
    std::string s = j.at("grade").get<std::string>();
    strcpy(h.grade, s.c_str());
}

class Scores
{
    public:
        static bool get_highscore(const std::string& checksum, Highscore& h) {
            if (scores.find(checksum) != scores.end()) {
                h = scores[checksum].get<Highscore>();
                return true;
            }
            return false;
        }

        static void set_highscore(Highscore& h) {
            scores[h.checksum] = h;
        }

        static void save() {
            char path[PATH_MAX];
            snprintf(path, PATH_MAX, "%s%s", DEF_DataDirectory, DEF_HighscoresFilename);

            std::ofstream o(path);
            o << std::setw(4) << scores << std::endl;
        }

        static void load() {
            scores = {};

            char path[PATH_MAX];
            snprintf(path, PATH_MAX, "%s%s", DEF_DataDirectory, DEF_HighscoresFilename);

            std::ifstream i(path);
            json j = json::parse(i, nullptr, false);

            if (j.is_discarded()) {
                fprintf(stderr, "Scores JSON parse error, proceeding with empty.\n");
                return;
            }

            scores = j;
        }

    private:
        static json scores;
};
