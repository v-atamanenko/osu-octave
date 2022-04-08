#include "BeatmapManager.h"
Beatmap* BeatmapManager::mBeatmapCurrent = nullptr;
std::vector<BeatmapEntry> BeatmapManager::mBeatmapsAll;
std::vector<BeatmapEntry> BeatmapManager::mBeatmapsFiltered;

void BeatmapManager::Load(std::string &checksum) {
	if (mBeatmapCurrent != nullptr)
		mBeatmapCurrent->CleanUp();

    BeatmapEntry bme;
    bool x = Beatmaps::get_beatmap(checksum, bme);
    if (!x) {
        // TODO: Display such errors to user
        fprintf(stderr, "Failed to get beatmap with checksum %s from index.\n", checksum.c_str());
        return;
    }

    mBeatmapCurrent = new Beatmap(bme.Filename, bme.BaseDir);
    if (!mBeatmapCurrent->Validate()) {
        // TODO: Display such errors to user
        fprintf(stderr, "Failed validation for beatmap with checksum %s from index.\n", checksum.c_str());
        fprintf(stderr, "Filename: %s. BaseDir: %s.\n", bme.Filename.c_str(), bme.BaseDir.c_str());
        return;
    }

	mBeatmapCurrent->Initialize();
}

void BeatmapManager::BuildMap() {
    Beatmaps::get_map(mBeatmapsAll);
    mBeatmapsFiltered = mBeatmapsAll;
}

void BeatmapManager::Filter(BeatmapFilter f) {
    switch (f) {
        case FILTER_NONE:
            mBeatmapsFiltered = mBeatmapsAll;
            break;
        case FILTER_ALPHA_A_E:
            mBeatmapsFiltered = Beatmaps::filter_character_range(mBeatmapsAll, 'A', 'E');
            break;
        case FILTER_ALPHA_F_J:
            mBeatmapsFiltered = Beatmaps::filter_character_range(mBeatmapsAll, 'F', 'J');
            break;
        case FILTER_ALPHA_K_O:
            mBeatmapsFiltered = Beatmaps::filter_character_range(mBeatmapsAll, 'K', 'O');
            break;
        case FILTER_ALPHA_P_T:
            mBeatmapsFiltered = Beatmaps::filter_character_range(mBeatmapsAll, 'P', 'T');
            break;
        case FILTER_ALPHA_U_Z:
            mBeatmapsFiltered = Beatmaps::filter_character_range(mBeatmapsAll, 'U', 'Z');
            break;
    }
}

bool BeatmapManager::CheckIndex() {
    char maps_path[PATH_MAX];
    snprintf(maps_path, PATH_MAX, "%s%s", DEF_DataDirectory, DEF_BeatmapsSubdirectory);

    DIR* dir = opendir(maps_path);
    struct dirent* entry;

    if (dir == nullptr) {
        fprintf(stderr, "Unable to open beatmaps dir (%s)!", maps_path);
        return false;
    }

    bool res = true;

    while ((entry = readdir(dir)) != nullptr) {
        //ignore generic names
        if (entry->d_name[0] == '.')
            continue;

        char map_subdir[PATH_MAX];
        snprintf(map_subdir, PATH_MAX, "%s%s%s", DEF_DataDirectory, DEF_BeatmapsSubdirectory, entry->d_name);

        DIR* subdir = opendir(map_subdir);

        //if this is a folder, find all the .ods files inside
        if (subdir != nullptr) {
            struct dirent* subentry;

            while ((subentry = readdir(subdir)) != nullptr) {
                DIR* filetest = opendir(subentry->d_name);

                if (filetest == nullptr) {
                    char* map_filename = subentry->d_name;
                    size_t length = strlen(map_filename);
                    if (length < 4)
                        continue;

                    char *ext = (char*)malloc(length + 1);
                    strcpy(ext, map_filename);

                    //if we have on our hands a .osu file, add it to our collection
                    if (strcmp(ext+(length-4), ".osu") == 0) {
                        if (!Beatmaps::is_path_in_state(map_filename, map_subdir)) {
                            res = false;
                        }
                    }

                    free(ext);
                } else {
                    closedir(filetest);
                }

                if (!res) {
                    break;
                }
            }

            closedir(subdir);
        }

        if (!res) {
            break;
        }
    }

    closedir(dir);

    std::vector<std::string> idx;
    Beatmaps::get_state(idx);

    for (auto &s : idx) {
        if (!file_exists(s)) {
            Beatmaps::remove_path_from_state(s);
            Beatmaps::remove_beatmap(s);
        }
    }

    return res;
}

void BeatmapManager::BuildCollection() {
    char maps_path[PATH_MAX];
    snprintf(maps_path, PATH_MAX, "%s%s", DEF_DataDirectory, DEF_BeatmapsSubdirectory);

    DIR* dir = opendir(maps_path);
	struct dirent* entry;

    if (dir == nullptr) {
        fprintf(stderr, "Unable to open beatmaps dir (%s)!", maps_path);
        return;
    }
	
	while ((entry = readdir(dir)) != nullptr) {
		//ignore generic names
		if (entry->d_name[0] == '.')
			continue;

        char map_subdir[PATH_MAX];
        snprintf(map_subdir, PATH_MAX, "%s%s%s", DEF_DataDirectory, DEF_BeatmapsSubdirectory, entry->d_name);

        DIR* subdir = opendir(map_subdir);
		
		//if this is a folder, find all the .ods files inside
		if (subdir != nullptr) {
			struct dirent* subentry;
			
			while ((subentry = readdir(subdir)) != nullptr) {
				DIR* filetest = opendir(subentry->d_name);
				
				if (filetest == nullptr) {
					char* map_filename = subentry->d_name;
                    size_t length = strlen(map_filename);
                    if (length < 4)
                        continue;

                    char *ext = (char*)malloc(length + 1);
                    strcpy(ext, map_filename);

					//if we have on our hands a .osu file, add it to our collection
					if (strcmp(ext+(length-4), ".osu") == 0) {
                        Add(map_filename, map_subdir);
					}

                    free(ext);
				} else {
					closedir(filetest);
				}
			}

            closedir(subdir);
		}
	}
	
	closedir(dir);
    Beatmaps::save();
}

void BeatmapManager::Add(const char* map_filename, const char* map_subdir) {
    Beatmaps::add_path_to_state(map_filename, map_subdir);
    BeatmapEntry bme;

    if (LoadEntryData(map_filename, map_subdir, bme)) {
        bme.BackgroundFilename = PreviewBuffer::GeneratePreview(map_subdir, bme.BackgroundFilename);
        bme.BackgroundFilepath = std::string(map_subdir) + "/" + bme.BackgroundFilename;
        Beatmaps::set_beatmap(bme);
    }
}

OOInt BeatmapManager::MapCount() {
	return (OOInt)BeatmapManager::Beatmaps().size();
}

OOInt BeatmapManager::SongCount() {
	//TODO: algorithm plz
	return (OOInt)BeatmapManager::Beatmaps().size();
}

bool BeatmapManager::LoadEntryData(const std::string &filename, const std::string &basedir, BeatmapEntry& bm) {
    std::ifstream file(basedir  + "/" + filename);
    if (!file) {
        fprintf(stderr, "Couldn't read .osu file %s\n", filename.c_str());
        return false;
    }

    osuParser::OsuParser p(&file);
    p.Parse();

    if (p.mode != osuParser::gmStandard) {
        // Not an osu! map
        return false;
    }

    bm.Filename = filename;
    bm.BaseDir = basedir;
    bm.Title = p.title;
    bm.Artist = p.artist;
    bm.Creator = p.creator;
    bm.Version = p.version;
    bm.AudioFilename = basedir + "/" + p.audioFilename;

    for (const osuParser::Event& e : p.events) {
        if (e.type == osuParser::eBackground) {
            bm.BackgroundFilename = e.file;
            bm.BackgroundFilepath = basedir + "/" + e.file;
        }
    }

    bm.Checksum = md5(p.title + p.artist + p.version + std::to_string(p.beatmapID));

    std::map<std::string, Output> sr = calculateStarRating(basedir  + "/" + filename);
    bm.starRating = sr["nomod"].total;
    return true;
}
