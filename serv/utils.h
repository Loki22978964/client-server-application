#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
namespace func{
std::string addSurname(const std::string& name);
std::string numbersMinMax(const std::string& numbers);
std::string add_arithmetic_mean(const std::string& numbers);
std::string checkPunctuation(const std::string& message);
}
#endif 