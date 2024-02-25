#pragma once
#include <array>
#include <conio.h>
#include <iostream>
#include <memory>

#ifdef _WIN32
#include <windows.h> // For Windows
#else
#include <cstdlib>   // For Linux and macOS
#endif

class helper
{
public:
    static void pause()
    {
        std::cout << "Press any key to continue..." << '\n';
        _getch();
    }

    static void clear()
    {
#ifdef _WIN32
        // For Windows
        COORD topLeft = { 0, 0 };
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO screen;
        DWORD written;

        GetConsoleScreenBufferInfo(console, &screen);
        FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
        FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
        SetConsoleCursorPosition(console, topLeft);
#else
        // For Linux and macOS
        std::cout << "\x1B[2J\x1B[H";
#endif
    }

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
