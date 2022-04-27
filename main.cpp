#include <filesystem>
#include <iostream>
#include <memory>

#include "FileSync.hpp"
#include "RegularExpressionHelpers.hpp"

int main()
{
    try
    {
        std::filesystem::path const pathSource("/home/user/Desktop/source");
        std::filesystem::path const pathDestination("/home/user/Desktop/backup");

        FileSync::FileSync(pathSource,
                           pathDestination,
                           /*includeFilters*/ {},
                           /*excludeFilters*/ {std::make_shared<FileSync::RegexDirectory>(".idea")})
                .backup();
    }
    catch (std::exception const & e)
    {
        std::cout << "Caught unexpected exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Caught unexpected exception: unknown exception." << std::endl;
    }
    return 0;
}
