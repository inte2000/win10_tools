#pragma once

#include <fstream>
#include <locale>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>
#include <memory>
#include <chrono>
#include <filesystem>
#include <cassert>
#include <ctime>

typedef std::basic_string<TCHAR> TString;

namespace stdfs = std::filesystem;  //C++ 17 support
//namespace stdfs = std::experiment::filesystem;  //for not full support C++17

