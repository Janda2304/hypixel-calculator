#ifndef STYLE_CONFIG_H
#define STYLE_CONFIG_H
#include <imgui.h>
#include <map>
#include <string>
#include <vector>

#include "color.hpp"


struct style_config
{
    static std::map<ImGuiCol, ImVec4> colors;
    static std::map<ImGuiStyleVar, float> style_vars;


    static void set_color(ImGuiCol color, ::color value);

    static void load_config(const std::string &path, ImGuiStyle &style);

    static void save_config(const std::string &path, const ImGuiStyle &style);

    static void serialize(const std::string& path);

    static void deserialize(const std::string& path);
};



#endif //STYLE_CONFIG_H
