#ifndef __LYON_UTIL_FS_H__
#define __LYON_UTIL_FS_H__

#include <fstream>
#include <ios>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

namespace lyon {

class FSUtil {
public:
    static std::string Dirname(const std::string &filename);
    static std::string Basename(const std::string &filename);
    static void ListAllFile(const std::string &path, const std::string &subfix,
                            std::vector<std::string> &files);

    static bool MakeDir(const std::string &dirname);

    static bool OpenForRead(std::ifstream &ifs, const std::string &filename,
                            std::ios_base::openmode mode);

    static bool OpenForWrite(std::ofstream &ofs, const std::string &filename,
                             std::ios_base::openmode mode);
};
} // namespace lyon
#endif // !__LYON_UTIL_FS_H__
