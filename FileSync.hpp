#ifndef FILE_SYNC_HPP
#define FILE_SYNC_HPP

#include <filesystem>

namespace FileSync
{

class FileSync
{
public:

    FileSync(std::filesystem::path const & source, std::filesystem::path const & destination);

    void backup();

private:

    static void backup_(std::filesystem::path const & source, std::filesystem::path const & destination);

    std::filesystem::path source_;
    std::filesystem::path destination_;

}; // class FileSync

} // namespace FileSync

#endif // FILE_SYNC_HPP
