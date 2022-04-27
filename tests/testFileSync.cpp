#include <iostream>

#include <FileSync.hpp>
#include <RegularExpressionHelpers.hpp>


namespace // anonymous
{

class FileSyncTest : public FileSync::FileSync
{
public:

    using FileSync::FileSync; // inherit all constructors

    using FileSync::checkFilters_;

}; // class FileSyncTest


std::vector<std::filesystem::path> prependPaths(std::vector<std::filesystem::path> const & paths, std::filesystem::path const & prepend)
{
    std::vector<std::filesystem::path> prependedPaths;
    prependedPaths.reserve(paths.size());
    for (std::filesystem::path const & path : paths)
    {
        prependedPaths.push_back(prepend / path);
    }
    return prependedPaths;
}

int checkFilters(FileSyncTest const & fileSyncTest,
                 std::vector<std::filesystem::path> const & expectTrue,
                 std::vector<std::filesystem::path> const & expectFalse)
{
    int returnCode = 0;

    for (std::filesystem::path const & filePath : expectTrue)
    {
        if (true != fileSyncTest.checkFilters_(filePath))
        {
            returnCode = 1;
        }
    }

    for (std::filesystem::path const & filePathRelative : expectFalse)
    {
        if (false != fileSyncTest.checkFilters_(filePathRelative))
        {
            returnCode = 1;
        }
    }

    return returnCode;
}


} // namespace anonymous


int main()
{
    int returnCode = 0;

    try
    {
        std::filesystem::path const filePathSource("/home/user/Desktop/source");
        std::filesystem::path const filePathDestination("/home/user/Desktop/destination");

        std::filesystem::path const filePathRelativePng("filename.png");
        std::filesystem::path const filePathRelativeJpg("filename.jpg");
        std::filesystem::path const filePathRelativeZip("subdirectory/filename.zip");
        std::filesystem::path const filePathRelativeTarGz("subdirectory/filename.tar.gz");
        std::filesystem::path const filePathRelativeAbc(".idea/filename.abc");

        {
            std::vector<FileSync::FileSync::FilterType> includeFilters;
            std::vector<FileSync::FileSync::FilterType> excludeFilters{
                std::make_shared<FileSync::RegexDirectory>(".idea"),
            };

            FileSyncTest const fileSyncTest(filePathSource,
                                            filePathDestination,
                                            includeFilters,
                                            excludeFilters);

            if (0 != checkFilters(fileSyncTest,
                                  prependPaths({filePathRelativePng, filePathRelativeJpg, filePathRelativeZip, filePathRelativeTarGz}, filePathSource),
                                  prependPaths({filePathRelativeAbc}, filePathSource)))
            {
                returnCode = 1;
            }
        }

        {
            std::vector<FileSync::FileSync::FilterType> includeFilters;
            std::vector<FileSync::FileSync::FilterType> excludeFilters{
                std::make_shared<FileSync::RegexDirectory>(".idea"),
                std::make_shared<FileSync::RegexFileName>(filePathRelativeJpg.string()),
                std::make_shared<FileSync::RegexFileExtension>("tar.gz"),
            };

            FileSyncTest const fileSyncTest(filePathSource,
                                            filePathDestination,
                                            includeFilters,
                                            excludeFilters);

            if (0 != checkFilters(fileSyncTest,
                                  prependPaths({filePathRelativePng, filePathRelativeZip, }, filePathSource),
                                  prependPaths({filePathRelativeAbc, filePathRelativeJpg, filePathRelativeTarGz, }, filePathSource)))
            {
                returnCode = 1;
            }
        }

        {
            std::vector<FileSync::FileSync::FilterType> includeFilters;
            std::vector<FileSync::FileSync::FilterType> excludeFilters{
                std::make_shared<FileSync::RegexFileName>(filePathRelativeJpg.string()),
            };

            FileSyncTest const fileSyncTest(filePathSource,
                                            filePathDestination,
                                            includeFilters,
                                            excludeFilters);

            if (0 != checkFilters(fileSyncTest,
                                  prependPaths({filePathRelativePng, filePathRelativeZip, filePathRelativeAbc, filePathRelativeTarGz, }, filePathSource),
                                  prependPaths({filePathRelativeJpg}, filePathSource)))
            {
                returnCode = 1;
            }
        }

        {
            std::vector<FileSync::FileSync::FilterType> includeFilters{
                std::make_shared<FileSync::RegexFileName>(filePathRelativeJpg.string()),
            };
            std::vector<FileSync::FileSync::FilterType> excludeFilters;

            FileSyncTest const fileSyncTest(filePathSource,
                                            filePathDestination,
                                            includeFilters,
                                            excludeFilters);

            if (0 != checkFilters(fileSyncTest,
                                  prependPaths({filePathRelativeJpg}, filePathSource),
                                  prependPaths({filePathRelativePng, filePathRelativeZip, filePathRelativeAbc, filePathRelativeTarGz, }, filePathSource)))
            {
                returnCode = 1;
            }
        }

        {
            std::vector<FileSync::FileSync::FilterType> includeFilters{
                std::make_shared<FileSync::RegexFileName>(filePathRelativeJpg.string()),
                std::make_shared<FileSync::RegexFileName>(filePathRelativePng.string()),
                std::make_shared<FileSync::RegexFileName>("filename.zip"),
            };
            std::vector<FileSync::FileSync::FilterType> excludeFilters{
                std::make_shared<FileSync::RegexDirectory>(".idea"),
                std::make_shared<FileSync::RegexFileExtension>("zip"), // this hides above "filename.zip"
            };

            FileSyncTest const fileSyncTest(filePathSource,
                                            filePathDestination,
                                            includeFilters,
                                            excludeFilters);

            if (0 != checkFilters(fileSyncTest,
                                  prependPaths({filePathRelativeJpg, filePathRelativePng, }, filePathSource),
                                  prependPaths({filePathRelativeZip, filePathRelativeAbc, filePathRelativeTarGz, }, filePathSource)))
            {
                returnCode = 1;
            }
        }

    }
    catch (std::exception const & e)
    {
        returnCode = 1;
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        returnCode = 1;
        std::cout << "unknown exception" << std::endl;
    }

    return returnCode;
}
