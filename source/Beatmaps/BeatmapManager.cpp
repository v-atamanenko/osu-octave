#include "BeatmapManager.h"

Beatmap* BeatmapManager::mBeatmapCurrent = nullptr;
std::vector<Beatmap*> BeatmapManager::mBeatmaps;

void BeatmapManager::Load(uint32_t index)
{
	if (mBeatmapCurrent != nullptr)
		mBeatmapCurrent->CleanUp();
	
	//Mode::ChangeToOsuDir();
	mBeatmapCurrent = mBeatmaps[index];
	mBeatmapCurrent->Initialize();
}

void BeatmapManager::BuildCollection()
{
	//Mode::ChangeToOsuDir();
	char* maps_path;
    SDL_asprintf(&maps_path, "%s%s", DATA_DIR, MAPS_DIR);

    DIR* dir = opendir(maps_path);
	struct dirent* entry;
	
	while ((entry = readdir(dir)) != nullptr)
	{
		//ignore generic names
		//if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		if (entry->d_name[0] == '.')
			continue;

        char* map_subdir;
        SDL_asprintf(&map_subdir, "%s%s%s", DATA_DIR, MAPS_DIR, entry->d_name);
		DIR* subdir = opendir(map_subdir);
		
		//if this is a folder, find all the .ods files inside
		if (subdir != nullptr)
		{
			struct dirent* subentry;
			
			while ((subentry = readdir(subdir)) != nullptr)
			{
				DIR* filetest = opendir(subentry->d_name);
				
				if (filetest == nullptr)
				{
					char* ext = subentry->d_name;
                    char* map_path;
                    SDL_asprintf(&map_path, "%s/%s", map_subdir, subentry->d_name);
					
					size_t length = strlen(ext);
					if (length < 4)
						continue;

                    //ext is now the extension of the current file
                    ext += length - 4;

					//if we have on our hands a .ods file, add it to our collection
					if (strcmp(ext, ".osu") == 0)
					{
						mBeatmaps.push_back(new Beatmap(map_path, map_subdir));
					}

                    free(map_path);
                    //free(ext);
				}
				else
				{
					closedir(filetest);
				}
			}

            closedir(subdir);
		}

        free(map_subdir);
	}
	
	closedir(dir);
    free(maps_path);
}

void BeatmapManager::Add(const char* map_path, const char* map_subdir) {
    auto* btmp = new Beatmap(map_path, map_subdir);
    if (btmp->Validate()) {
        mBeatmaps.push_back(btmp);
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