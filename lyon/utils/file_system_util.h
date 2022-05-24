#ifndef __LYON_UTILS_FS_H__
#define __LYON_UTILS_FS_H__

#include <fstream>
#include <ios>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

namespace lyon {

class FSUtil {
public:
    /**
     * @brief 获取文件名的文件夹路径
     *
     * @param filename 文件名
     */
    static std::string Dirname(const std::string &filename);

    /**
     * @brief 获取文件名的单文件名
     *
     * @param filename 文件名
     */
    static std::string Basename(const std::string &filename);

    /**
     * @brief 列出path路径下所有以subfix结尾的常规文件
     *
     * @param path 路径
     * @param subfix 需要的文件后缀
     * @param files 最终文件保存的位置
     */
    static void ListAllFile(const std::string &path, const std::string &subfix,
                            std::vector<std::string> &files);

    /**
     * @brief 递归创建文件夹
     *
     * @param dirname 文件夹名
     * @return 文件夹创建是否成功
     */
    static bool MakeDir(const std::string &dirname);

    /**
     * @brief 打开读文件流
     *
     * @param ifs 文件流
     * @param filename 需要读的文件地址
     * @param mode 打开文件的模式
     * @return 文件流打开是否成功
     */
    static bool OpenForRead(std::ifstream &ifs, const std::string &filename,
                            std::ios_base::openmode mode);

    /**
     * @brief 打开写文件流
     *
     * @param ofs 文件流
     * @param filename 需要写的文件地址
     * @param mode 打开文件的模式
     * @return 文件流打开是否成功
     */
    static bool OpenForWrite(std::ofstream &ofs, const std::string &filename,
                             std::ios_base::openmode mode);

    static bool Unlink(const std::string &filename, bool exist);
};
} // namespace lyon
#endif // !__LYON_UTIL_FS_H__
