#include "Betmaps.h"
#include "Scores.h"
#include "Settings.h"
#include "Skins.h"

json Settings::settings;
json Scores::scores;
json Beatmaps::beatmaps;
json Beatmaps::state;

json Skins::skins;
bool Skins::loaded = false;

Skin Skins::skin_options;
std::map<HitSoundName, std::string> Skins::gameplay_sounds;
std::map<UISoundName, std::string> Skins::ui_sounds;
std::map<TextureType, std::string> Skins::textures;
std::map<FontName, std::string> Skins::fonts;
