#include "FileSync.hpp"

#include <algorithm>
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

std::filesystem::path projectedPath(std::filesystem::path const & pathRelative,
                                    std::filesystem::path const & baseDestination)
{
    return baseDestination / pathRelative;
}

void deleteFromVector(std::vector<FileSync::FileSync::FilterType> & collection,
                      std::vector<FileSync::FileSync::FilterType> const & remove)
{
    for (FileSync::FileSync::FilterType const & filter : remove)
    {
        if (std::find(collection.cbegin(), collection.cend(), filter) == collection.cend())
        {
            throw std::runtime_error("At least one filter to delete does not exist.");
        }
    }

    for (FileSync::FileSync::FilterType const & filter : remove)
    {
        collection.erase(std::find(collection.cbegin(), collection.cend(), filter));
    }
}

} // namespace anonymous

namespace FileSync
{

FileSync::FileSync(std::filesystem::path const & source,
                   std::filesystem::path const & destination)
    : FileSync(source, destination, {}, {})
{
    // intentionally empty
}

FileSync::FileSync(std::filesystem::path const & source,
                   std::filesystem::path const & destination,
                   std::vector<std::shared_ptr<std::regex>> && includeFilters,
                   std::vector<std::shared_ptr<std::regex>> && excludeFilters)
    : source_(source)
    , destination_(destination)
    , includeFilters_(std::move(includeFilters))
    , excludeFilters_(std::move(excludeFilters))
{
    // intentionally empty
}

FileSync::FileSync(std::filesystem::path const & source,
                   std::filesystem::path const & destination,
                   std::vector<std::shared_ptr<std::regex>> const & includeFilters,
                   std::vector<std::shared_ptr<std::regex>> const & excludeFilters)
    : FileSync(source,
               destination,
               std::vector<std::shared_ptr<std::regex>>(includeFilters),
               std::vector<std::shared_ptr<std::regex>>(excludeFilters))
{
    // intentionally empty
}


void FileSync::addIncludeFilters(std::vector<FilterType> const & filters)
{
    includeFilters_.insert(includeFilters_.cend(), filters.cbegin(), filters.cend());
}

void FileSync::removeIncludeFilters(std::vector<FilterType> const & filters)
{
    deleteFromVector(includeFilters_, filters);
}

std::vector<FileSync::FilterType> const & FileSync::getIncludeFilters() const
{
    return includeFilters_;
}


void FileSync::addExcludeFilters(std::vector<FileSync::FilterType> const & filters)
{
    excludeFilters_.insert(excludeFilters_.cend(), filters.cbegin(), filters.cend());
}

void FileSync::removeExcludeFilters(std::vector<FileSync::FilterType> const & filters)
{
    deleteFromVector(excludeFilters_, filters);
}

std::vector<FileSync::FilterType> const & FileSync::getExcludeFilters() const
{
    return excludeFilters_;
}


void FileSync::backup()
{
    backup_(source_, destination_);
}


bool FileSync::checkFilters_(std::filesystem::path const & path) const
{
    bool fileToInclude = true;

    if (/*fileToInclude && */!includeFilters_.empty())
    {
        bool anyIncludeFilterMatched = false;
        for (FileSync::FilterType const & filter : includeFilters_)
        {
            if (std::regex_search(path.string(), *filter))
            {
                anyIncludeFilterMatched = true;
                break;
            }
        }
        fileToInclude = anyIncludeFilterMatched;
    }

    // If explicit includes are specified and this path did not match,
    // then we don't need to check excludes - otherwise we do.
    if (fileToInclude && !excludeFilters_.empty())
    {
        bool anyExcludeFilterMatched = false;
        for (FileSync::FilterType const & filter : excludeFilters_)
        {
            if (std::regex_search(path.string(), *filter))
            {
                anyExcludeFilterMatched = true;
                break;
            }
        }
        fileToInclude = !anyExcludeFilterMatched;
    }

    return fileToInclude;
}


void FileSync::backup_(std::filesystem::path const & source, std::filesystem::path const & destination) const
{
    // First backup all files and subdirectories from source anew.
    for (std::filesystem::directory_entry const & entry : std::filesystem::directory_iterator(source))
    {
        std::filesystem::path const pathRelative = relativePath(entry.path(), source);
        bool const includedByFilters = checkFilters_(pathRelative);
        if (includedByFilters)
        {
            std::filesystem::path const pathEntryDestination = projectedPath(pathRelative, destination);

            if (entry.is_regular_file())
            {
                bool const success = std::filesystem::copy_file(entry.path(), pathEntryDestination, std::filesystem::copy_options::update_existing);
                std::cout << "file: " << entry.path() << " - " << (success ? "copied" : "not copied") << std::endl;
            }
            else if (entry.is_directory())
            {
                std::filesystem::file_status const destinationFileStatus = std::filesystem::status(pathEntryDestination);
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

                backup_(entry.path(), pathEntryDestination);
            }
            else
            {
                std::cout << "unknown file type: " << entry.path() << std::endl;
            }
        }
        else
        {
            std::cout << "file: " << entry.path() << " - not included / excluded by filters" << std::endl;
        }
    }

    // Afterwards clean up and remove all retained files/directories.
    for (std::filesystem::directory_entry const & entry : std::filesystem::directory_iterator(destination))
    {
        if (entry.is_regular_file() || entry.is_directory())
        {
            bool deleteBackupedFile = false;
            std::filesystem::path const pathRelative = relativePath(entry.path(), destination);

            // First check whether [potentially new] filters exclude the backuped file from backup.
            {
                bool const includedByFilters = checkFilters_(pathRelative);
                deleteBackupedFile = !includedByFilters;
            }

            // If not excluded, check whether source still exists.
            if (!deleteBackupedFile)
            {
                std::filesystem::path const pathEntrySource = projectedPath(pathRelative, source);
                deleteBackupedFile = !std::filesystem::exists(pathEntrySource);
            }

            // Now delete - if required.
            if (deleteBackupedFile)
            {
                std::filesystem::remove_all(entry.path());
            }
        }
        else
        {
            std::cout << "unknown file type: " << entry.path() << std::endl;
        }
    }
}

} // namespace FileSync
