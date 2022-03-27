#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>

namespace Utils
{
    inline std::string LoadText(const std::string &path)
    {
        std::ifstream file;
        file.open(path);
        assert(file.is_open());

        std::stringstream sstr;
        sstr << file.rdbuf();
        file.close();
        return sstr.str();
    }
};
