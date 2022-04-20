#ifndef REGULAR_EXPRESSION_HELPERS_HPP
#define REGULAR_EXPRESSION_HELPERS_HPP

#include <regex>
#include <string>

namespace FileSync
{

class RegexFileName : public std::regex
{
public:
    RegexFileName(std::string const & filename);

}; // class RegexFilename

class RegexFileExtension : public std::regex
{
public:
    RegexFileExtension(std::string const & extension);

}; // class RegexFileExtension

class RegexDirectory : public std::regex
{
public:
    RegexDirectory(std::string const & directory);
    RegexDirectory(std::initializer_list<std::string> const directories);

}; // class RegexFileExtension

} // namespace FileSync

#endif // REGULAR_EXPRESSION_HELPERS_HPP
