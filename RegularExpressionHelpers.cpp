#include "RegularExpressionHelpers.hpp"

#include <exception>
#include <sstream>

static char constexpr fileSeperator = '/';
static char constexpr filenameInvalidCharacters[] =
        #if defined(WIN32)
            {'\0', '\\', '/', ':', '*', '?', '"', '<', '>', '|'};
        #elif __unix__
            {'\0', fileSeperator};
        #elif __APPLE__
            {':', fileSeperator};
        #else
            #error "Unknown operating system."
        #endif

static bool checkFilenameValid(std::string const & filename)
{
    bool filenameValid = true;
    for (char const character : filenameInvalidCharacters)
    {
        if (filename.find(character) != std::string::npos)
        {
            filenameValid = false;
            break;
        }
    }
    return filenameValid;
}


using namespace std::literals; // required for operator""s

namespace FileSync
{

RegexFilename::RegexFilename(std::string const & filename)
    : std::regex("^.*"s + filename + "$"s)
{
    if (!checkFilenameValid(filename))
    {
        std::stringstream message;
        message << "RegexFilename: Invalid filename supplied: \"" << filename << "\".";
        throw std::runtime_error(message.str());
    }
}

} // namespace FileSync
