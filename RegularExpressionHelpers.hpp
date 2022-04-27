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
    RegexFileName(RegexFileName const & other) = default;
    RegexFileName(RegexFileName && other) = default;

}; // class RegexFilename

class RegexFileExtension : public std::regex
{
public:
    RegexFileExtension(std::string const & extension);
    RegexFileExtension(RegexFileExtension const & other) = default;
    RegexFileExtension(RegexFileExtension && other) = default;

}; // class RegexFileExtension

class RegexDirectory : public std::regex
{
public:
    RegexDirectory(std::string const & directory);
    RegexDirectory(std::initializer_list<std::string> const directories);
    RegexDirectory(RegexDirectory const & other) = default;
    RegexDirectory(RegexDirectory && other) = default;

}; // class RegexFileExtension

} // namespace FileSync

#endif // REGULAR_EXPRESSION_HELPERS_HPP
