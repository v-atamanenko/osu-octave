#pragma once

#include <sys/stat.h>
#include <dirent.h>

inline bool file_exists(const std::string& fname) {
    struct stat buffer{};
    return (stat (fname.c_str(), &buffer) == 0);
}

inline bool is_dir(char* p) {
    DIR* filetest = opendir(p);
    if (filetest != nullptr) {
        closedir(filetest);
        return true;
    }
    return false;
}

inline bool get_files_in_directory(const std::string& path, std::vector<std::string>& v, bool strip_extension = false) {
    DIR *dir;
    struct dirent *ent;

    v.clear();

    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            if (ent->d_name[0] != '.') {
                if (strip_extension) {
                    std::string s = ent->d_name;
                    size_t lastindex = s.find_last_of(".");
                    if (lastindex != std::string::npos && lastindex != 0) {
                        s = s.substr(0, lastindex);
                    }
                    v.emplace_back(s);
                } else {
                    v.emplace_back(ent->d_name);
                }
            }
        }
        closedir (dir);
    } else {
        return false;
    }

    return true;
}
