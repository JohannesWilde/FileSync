#ifndef REGULAR_EXPRESSION_HELPERS_HPP
#define REGULAR_EXPRESSION_HELPERS_HPP

#include <regex>
#include <string>

namespace FileSync
{

class RegexFilename : public std::regex
{
public:
    RegexFilename(std::string const & filename);

}; // class RegexFilename

} // namespace FileSync

#endif // REGULAR_EXPRESSION_HELPERS_HPP
