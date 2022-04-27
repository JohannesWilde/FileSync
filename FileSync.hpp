#ifndef FILE_SYNC_HPP
#define FILE_SYNC_HPP

#include <filesystem>
#include <memory>
#include <regex>
#include <vector>

namespace FileSync
{

/**
 * @brief The FileSync class provides an interface for backing up files from one directory to an other.
 *
 * Filters are provided for finer control of what shall be backed up:
 *  - includeFilters: Only files explicitely specified here will be backed up.
 *  - excludeFilters: Anything matching these won't be backed up.
 * Please note that the exludeFilters have precedence, i.e. overwrite matching include filters.
 * These filters only work on relative paths as to be consistent in the source and backup directory,
 * no matter where these actually are.
 *
 */
class FileSync
{
public:

    typedef std::shared_ptr<std::regex> FilterType;

    FileSync(std::filesystem::path const & source,
             std::filesystem::path const & destination);

    FileSync(std::filesystem::path const & source,
             std::filesystem::path const & destination,
             std::vector<FilterType> && includeFilters,
             std::vector<FilterType> && excludeFilters);

    FileSync(std::filesystem::path const & source,
             std::filesystem::path const & destination,
             std::vector<FilterType> const & includeFilters,
             std::vector<FilterType> const & excludeFilters);

    void backup();

    void addIncludeFilters(std::vector<FilterType> const & filters);
    void removeIncludeFilters(std::vector<FilterType> const & filters);
    std::vector<FilterType> const & getIncludeFilters() const;

    void addExcludeFilters(std::vector<FilterType> const & filters);
    void removeExcludeFilters(std::vector<FilterType> const & filters);
    std::vector<FilterType> const & getExcludeFilters() const;

protected:

    void backup_(std::filesystem::path const & source, std::filesystem::path const & destination) const;

    /**
     * @brief checkFilters Check include and exclude filters - return true if it is to be backed up.
     * @param path Path to check.
     * @return true if included, false if excluded.
     *
     * Please note that exclude filters will have precedence - i.e. any includeFilter can be negated by
     * a corresponding exclude filter.
     *
     * If no filters are specified [include or exclude respectively] then everything will be included.
     *
     */
    bool checkFilters_(std::filesystem::path const & path) const;

private:

    std::filesystem::path source_;
    std::filesystem::path destination_;

    std::vector<std::shared_ptr<std::regex>> includeFilters_;
    std::vector<std::shared_ptr<std::regex>> excludeFilters_;

}; // class FileSync

} // namespace FileSync

#endif // FILE_SYNC_HPP
