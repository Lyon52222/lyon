#include "file_system_util.h"
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
namespace lyon {

std::string FSUtil::Dirname(const std::string &filename) {
    if (filename.empty()) {
        return ".";
    }
    auto pos = filename.rfind('/');
    if (pos == 0) {
        return "/";
    } else if (pos == std::string::npos) {
        return ".";
    } else {
        return filename.substr(0, pos);
    }
}

std::string FSUtil::Basename(const std::string &filename) {
    if (filename.empty()) {
        return filename;
    }
    auto pos = filename.rfind('/');
    if (pos == std::string::npos) {
        return filename;
    } else {
        return filename.substr(pos + 1);
    }
}

void FSUtil::ListAllFile(const std::string &path, const std::string &subfix,
                         std::vector<std::string> &files) {
    if (access(path.c_str(), 0) != 0) {
        return;
    }
    DIR *dir = opendir(path.c_str());
    if (dir == nullptr)
        return;
    struct dirent *dp = nullptr;
    while ((dp = readdir(dir)) != nullptr) {
        if (dp->d_type == DT_DIR) {
            if (!strcmp(dp->d_name, ".") || (!strcmp(dp->d_name, "..")))
                continue;

            ListAllFile(path + "/" + dp->d_name, subfix, files);
        } else if (dp->d_type == DT_REG) {
            std::string filename(dp->d_name);
            if (subfix.empty()) {
                files.push_back(path + "/" + filename);
            } else {
                if ((filename.length() >= subfix.length()) &&
                    (filename.substr(filename.length() - subfix.length()) ==
                     subfix)) {
                    files.push_back(path + "/" + filename);
                }
            }
        }
    }
    closedir(dir);
}

/**
 * @brief 获取文件的描述信息
 *
 * @param file 文件名
 * @param st 文件的描述信息
 * @return 文件描述信息获取是否成功
 */
static bool __lstat(const char *file, struct stat *st = nullptr) {
    struct stat lst;
    int ret = lstat(file, &lst);
    if (st) {
        *st = lst;
    }
    return ret;
}

/**
 * @brief 创建文件夹
 *
 * @param dirname 文件夹名
 * @return 创建是否成功
 */
static bool __mkdir(const char *dirname) {
    if (access(dirname, F_OK) == 0) {
        return 0;
    }
    return mkdir(dirname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

// FIX: 无法创建绝对路线下的文件，比如"/apps/logs/log.txt"
bool FSUtil::MakeDir(const std::string &dirname) {
    if (__lstat(dirname.c_str()) == 0) {
        return true;
    }
    char *path = strdup(dirname.c_str());
    char *ptr = strchr(path + 1, '/');
    do {
        for (; ptr; *ptr = '/', ptr = strchr(ptr + 1, '/')) {
            *ptr = '\0';
            if (__mkdir(path) != 0) {
                break;
            }
        }
        if (ptr != nullptr) {
            break;
        } else if (__mkdir(path) != 0) {
            break;
        }
        free(path);
        return true;
    } while (0);
    return false;
}

bool FSUtil::OpenForRead(std::ifstream &ifs, const std::string &filename,
                         std::ios_base::openmode mode) {
    ifs.open(filename.c_str(), mode);
    return ifs.is_open();
}

bool FSUtil::OpenForWrite(std::ofstream &ofs, const std::string &filename,
                          std::ios_base::openmode mode) {
    ofs.open(filename.c_str(), mode);
    if (!ofs.is_open()) {
        std::string dir = Dirname(filename);
        MakeDir(dir);
        ofs.open(filename.c_str(), mode);
    }
    return ofs.is_open();
}

bool FSUtil::Unlink(const std::string &filename, bool exist) {
    if (!exist && __lstat(filename.c_str())) {
        return true;
    }
    return ::unlink(filename.c_str());
}
} // namespace lyon
