#pragma once
#include <array>
#include <memory>



class helper
{
public:
    static std::string exec(const char* cmd)
    {
        std::array<char, 128> buffer{};
        std::string result;
        std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
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
    
};
