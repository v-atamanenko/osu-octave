#include "BeatmapManager.h"

Beatmap* BeatmapManager::mBeatmapCurrent = NULL;
std::vector<Beatmap*> BeatmapManager::mBeatmaps;

void BeatmapManager::Load(u32 index)
{
	if (mBeatmapCurrent != NULL)
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
	
	while ((entry = readdir(dir)) != NULL)
	{
		//ignore generic names
		//if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		if (entry->d_name[0] == '.')
			continue;
		
		chdir(entry->d_name);
		DIR* subdir = opendir(".");
		
		//if this is a folder, find all the .ods files inside
		if (subdir != NULL)
		{
			struct dirent* subentry;
			
			while ((subentry = readdir(subdir)) != NULL)
			{
				DIR* filetest = opendir(subentry->d_name);
				
				if (filetest == NULL)
				{
					char* ext = subentry->d_name;
					
					int length = strlen(ext);
					if (length < 4)
						continue;

                    nocashMessage(ext);

                    //ext is now the extension of the current file
                    ext += length - 4;

					//if we have on our hands a .ods file, add it to our collection
					if (strcmp(ext, ".ods") == 0)
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

u32 BeatmapManager::MapCount()
{
	return 8;//mBeatmaps.size();
}

u32 BeatmapManager::SongCount()
{
	//TODO: algorithm plz
	return mBeatmaps.size();
}