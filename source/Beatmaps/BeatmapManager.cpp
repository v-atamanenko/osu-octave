#include "BeatmapManager.h"

Beatmap* BeatmapManager::mBeatmapCurrent = NULL;
std::vector<Beatmap*> BeatmapManager::mBeatmaps;

void BeatmapManager::Load(uint32_t index)
{
	if (mBeatmapCurrent != nullptr)
		mBeatmapCurrent->CleanUp();
	
	Mode::ChangeToOsuDir();
	mBeatmapCurrent = mBeatmaps[index];
	mBeatmapCurrent->Initialize();
}

void BeatmapManager::BuildCollection()
{
	Mode::ChangeToOsuDir();
	
	DIR* dir = opendir(".");
	struct dirent* entry;
	
	while ((entry = readdir(dir)) != nullptr)
	{
		//ignore generic names
		//if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		if (entry->d_name[0] == '.')
			continue;
		
		chdir(entry->d_name);
		DIR* subdir = opendir(".");
		
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
					
					size_t length = strlen(ext);
					if (length < 4)
						continue;

                    //ext is now the extension of the current file
                    ext += length - 4;

					//if we have on our hands a .ods file, add it to our collection
					if (strcmp(ext, ".osu") == 0)
					{
						mBeatmaps.push_back(new Beatmap(subentry->d_name, entry->d_name));
					}
				}
				else
				{
					closedir(filetest);
				}
			}
		}
		
		closedir(subdir);
		chdir("..");
	}
	
	closedir(dir);
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