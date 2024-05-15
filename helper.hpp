#pragma once
#include <array>
#include <fstream>

#include <iostream>
#include <memory>
#include <imgui.h>

#ifdef _WIN32
#include <windows.h> // For Windows
#include <conio.h>
#else
#include <cstdlib>   // For Linux and macOS
#endif

class helper
{
public:
    static void pause()
    {
#ifdef _WIN32
        std::cout << "Press any key to continue..." << '\n';
        _getch();
#endif
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



    static void loadStyleColorsFromConfig(ImGuiStyle& style)
    {
        std::map<std::string, ImGuiCol_> style_map =
        {
            {"Text", ImGuiCol_Text},
            {"WindowBg", ImGuiCol_WindowBg},
            {"Button", ImGuiCol_Button},
            {"Header", ImGuiCol_Header},
        };
        std::ifstream config_file("config.json");
        nlohmann::json config;
        config_file >> config;

        for (auto& element : config.items())
            {
            const std::string& styleName = element.key();
            std::vector<float> colorValues = element.value();

            if (style_map.count(styleName) > 0) {

                ImGuiCol_ styleEnum = style_map[styleName];
                ImVec4 color = ImVec4(colorValues[0], colorValues[1], colorValues[2], colorValues[3]);
                style.Colors[styleEnum] = color;
            }
        }
    }
    
};
