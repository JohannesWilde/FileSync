#include "FileSync.hpp"

#include <iostream>

namespace // anonymous
{

std::filesystem::path relativePath(std::filesystem::path const & path,
                                   std::filesystem::path const & base)
{
    return path.lexically_relative(base);
}

std::filesystem::path projectedPath(std::filesystem::path const & path,
                                    std::filesystem::path const & baseSource,
                                    std::filesystem::path const & baseDestination)
{
    return baseDestination / relativePath(path, baseSource);
}

} // namespace anonymous

namespace FileSync
{

void backup(std::filesystem::path const & source, std::filesystem::path const & destination)
{
    for (std::filesystem::directory_entry const & entry : std::filesystem::recursive_directory_iterator(source))
    {
        std::filesystem::path const pathEntryDestination = projectedPath(entry.path(), source, destination);

        if (entry.is_regular_file())
        {
            bool const success = std::filesystem::copy_file(entry.path(), pathEntryDestination);
            std::cout << "file: " << entry.path() << " - " << (success ? "ok" : "error") << std::endl;
        }
        else if (entry.is_directory())
        {
            bool const success = std::filesystem::create_directory(pathEntryDestination);
            std::cout << "directory: " << entry.path() << " - " << (success ? "ok" : "error") << std::endl;
        }
    }
}

} // namespace FileSync
