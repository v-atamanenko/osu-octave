#include "BeatmapManager.h"

Beatmap* BeatmapManager::mBeatmapCurrent = nullptr;
std::vector<BeatmapEntry> BeatmapManager::mBeatmaps;

void BeatmapManager::Load(std::string &checksum)
{
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
    Beatmaps::get_map(mBeatmaps);
}

bool BeatmapManager::CheckIndex() {
    char* maps_path;
    SDL_asprintf(&maps_path, "%s%s", DEF_DataDirectory, DEF_BeatmapsSubdirectory);

    DIR* dir = opendir(maps_path);
    struct dirent* entry;

    bool res = true;

    while ((entry = readdir(dir)) != nullptr) {
        //ignore generic names
        if (entry->d_name[0] == '.')
            continue;

        char* map_subdir;
        SDL_asprintf(&map_subdir, "%s%s%s", DEF_DataDirectory, DEF_BeatmapsSubdirectory, entry->d_name);
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

        free(map_subdir);

        if (!res) {
            break;
        }
    }

    closedir(dir);
    free(maps_path);

    return res;
}

void BeatmapManager::BuildCollection() {
	char* maps_path;
    SDL_asprintf(&maps_path, "%s%s", DEF_DataDirectory, DEF_BeatmapsSubdirectory);

    DIR* dir = opendir(maps_path);
	struct dirent* entry;
	
	while ((entry = readdir(dir)) != nullptr) {
		//ignore generic names
		if (entry->d_name[0] == '.')
			continue;

        char* map_subdir;
        SDL_asprintf(&map_subdir, "%s%s%s", DEF_DataDirectory, DEF_BeatmapsSubdirectory, entry->d_name);
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
                        Beatmaps::add_path_to_state(map_filename, map_subdir);
					}

                    free(ext);
				} else {
					closedir(filetest);
				}
			}

            closedir(subdir);
		}

        free(map_subdir);
	}
	
	closedir(dir);
    free(maps_path);
    Beatmaps::save();
}

void BeatmapManager::Add(const char* map_filename, const char* map_subdir) {
    BeatmapEntry bme;

    if (Beatmap::LoadEntryData(map_filename, map_subdir, bme)) {
        Beatmaps::set_beatmap(bme);
    }
}

uint32_t BeatmapManager::MapCount()
{
	return mBeatmaps.size();
}

uint32_t BeatmapManager::SongCount()
{
	//TODO: algorithm plz
	return mBeatmaps.size();
}