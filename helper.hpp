﻿#pragma once
#include <array>
#include <memory>
#include <imgui.h>
#include <unistd.h>
#include <nlohmann/json.hpp>


struct myseps : std::numpunct<char>
{
    /* use space as separator */
    char do_thousands_sep() const { return ' '; }

    /* digits are grouped by 3 digits each */
    std::string do_grouping() const { return "\3"; }
};

class helper
{
public:
    static std::string exec(const char* cmd)
    {
        std::array<char, 128> buffer{};
        std::string result;
        std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
        if (!pipe)
        {
            throw std::runtime_error("popen() failed!");
        }
        while (!feof(pipe.get()))
        {
            if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            {
                result += buffer.data();
            }
        }
        return result;
    }

    static std::string get_exe_path()
    {
        char result[1024];
        ssize_t count = readlink("/proc/self/exe", result, 1024);
        return std::string(result, (count > 0) ? count : 0);
    }

    static std::string format_numbers(int value)
    {
        std::stringstream ss;
        ss.imbue(std::locale(std::locale(), new myseps));
        ss << std::fixed << value;
        return ss.str();
    }

    static std::string format_numbers(float value)
    {
        std::stringstream ss;
        ss.imbue(std::locale(std::locale(), new myseps));
        ss << std::fixed << value;
        return ss.str();
    }
};


