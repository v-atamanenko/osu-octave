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

inline bool get_files_in_directory(const std::string& path, std::vector<std::string>& v) {
    DIR *dir;
    struct dirent *ent;

    v.clear();

    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            v.emplace_back(ent->d_name);
        }
        closedir (dir);
    } else {
        return false;
    }

    return true;
}
