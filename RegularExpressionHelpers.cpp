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

RegexFileName::RegexFileName(std::string const & filename)
    : std::regex("^(.*\\"s + fileSeperator + ")?"s + filename + "$"s)
{
    if (!checkFilenameValid(filename))
    {
        std::stringstream message;
        message << "RegexFileName: Invalid filename supplied: \"" << filename << "\".";
        throw std::runtime_error(message.str());
    }
}

RegexFileExtension::RegexFileExtension(std::string const & extension)
    : std::regex("^.*\\."s + extension + "$"s)
{
    if ((extension.size() > 0) && ('.' == extension.at(0)))
    {
        std::stringstream message;
        message << "RegexFileExtension: The leading dot shall not be specified: \"" << extension << "\".";
        throw std::runtime_error(message.str());
    }
}

RegexDirectory::RegexDirectory(std::string const & directory)
    : RegexDirectory({directory})
{
    // intentionally empty
}

RegexDirectory::RegexDirectory(std::initializer_list<std::string> const directories)
    : std::regex([directories]()
    {
        std::stringstream expression;
        expression << "^(.*\\" << fileSeperator << ")?("s;
        if (0 < directories.size())
        {
            for (std::initializer_list<std::string>::const_iterator directoryIterator = directories.begin();
                 std::prev(directories.end()) != directoryIterator;
                 ++directoryIterator)
            {
                expression << *directoryIterator << "\\" << fileSeperator;
            }
            expression << *std::prev(directories.end()) << ")(\\" << fileSeperator << ".+)?";
        }
        expression << "$";
        return expression.str();
    } ()
    )
{
    // intentionally empty
}

} // namespace FileSync
