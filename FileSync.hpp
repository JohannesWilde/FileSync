#ifndef FILE_SYNC_HPP
#define FILE_SYNC_HPP

#include <filesystem>

namespace FileSync
{

    void backup(std::filesystem::path const & source, std::filesystem::path const & destination);

} // namespace FileSync

#endif // FILE_SYNC_HPP
