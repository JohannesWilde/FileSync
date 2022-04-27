#include <filesystem>
#include <iostream>

#include "FileSync.hpp"

int main()
{
    try
    {
        std::filesystem::path const pathSource("/home/user/Desktop/source");
        std::filesystem::path const pathDestination("/home/user/Desktop/backup");

        FileSync::FileSync(pathSource, pathDestination).backup();
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
