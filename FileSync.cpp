#include "FileSync.hpp"

#include <exception>
#include <iostream>
#include <sstream>

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
    for (std::filesystem::directory_entry const & entry : std::filesystem::directory_iterator(source))
    {
        std::filesystem::path const pathEntryDestination = projectedPath(entry.path(), source, destination);

        std::filesystem::file_status const destinationFileStatus = std::filesystem::status(pathEntryDestination);

        if (entry.is_regular_file())
        {
            bool const success = std::filesystem::copy_file(entry.path(), pathEntryDestination);
            std::cout << "file: " << entry.path() << " - " << (success ? "ok" : "error") << std::endl;
        }
        else if (entry.is_directory())
        {
            bool destinationDirectoryExists = std::filesystem::exists(destinationFileStatus);
            if (destinationDirectoryExists)
            {
                if (!std::filesystem::is_directory(destinationFileStatus))
                {
                    std::filesystem::remove(pathEntryDestination);
                    destinationDirectoryExists = false;
                }
            }
            // Now recheck whether I just deleted it.
            if (!destinationDirectoryExists)
            {
                bool const success = std::filesystem::create_directory(pathEntryDestination);
                if (!success)
                {
                    std::stringstream message;
                    message << "Failed to create directory: " << pathEntryDestination << std::endl;
                    throw std::runtime_error(message.str());
                }
            }

            backup(entry.path(), pathEntryDestination);
        }
        else
        {
            std::cout << "unknown file type: " << entry.path() << std::endl;
        }
    }
}

} // namespace FileSync
