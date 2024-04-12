#include "style_config.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

std::map<ImGuiCol, ImVec4> style_config::colors;

void style_config::set_color(ImGuiCol color, ::color value)
{
    colors.insert_or_assign(color, value);
}

void style_config::load_config(const std::string &path, ImGuiStyle& style)
{
    deserialize(path);
    for (const auto& element : colors)
    {
        style.Colors[element.first] = element.second;
    }
}

void style_config::save_config(const std::string &path, const ImGuiStyle& style)
{
    for (int i = 0; i < ImGuiCol_COUNT; i++)
    {
        colors.insert_or_assign(static_cast<ImGuiCol>(i), style.Colors[i]);
    }
    serialize(path);
}

void style_config::serialize(const std::string &path)
{
    nlohmann::json json_data;
    for (const auto& [key, value] : colors)
    {
        color c(value);
        json_data[std::to_string(key)] = c.serialize();
    }
    std::ofstream file (path);
    file << json_data.dump(4);
    file.close();
}

void style_config::deserialize(const std::string &path)
{
    nlohmann::json json_data;
    std::ifstream file(path);
    file >> json_data;
    file.close();
    for (const auto& [key, value] : json_data.items())
    {
        colors.insert_or_assign(static_cast<ImGuiCol>(std::stoi(key)), color::deserialize(value));
    }
}
