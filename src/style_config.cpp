#include "style_config.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

std::map<ImGuiCol, ImVec4> style_config::colors;
std::map<ImGuiStyleVar, float> style_config::style_vars;

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

    for (auto style_pair: style_vars)
    {
       ImGui::PushStyleVar(style_pair.first, style_pair.second);
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
    nlohmann::json json_colors;
    nlohmann::json json_style_vars;

    for (const auto& [key, value] : colors)
    {
        color c(value);
        json_colors[std::to_string(key)] = c.serialize();
    }

    for (auto vars: style_vars)
    {
        json_style_vars[std::to_string(vars.first)] = vars.second;
    }

    json_data["colors"] = json_colors;
    json_data["style_vars"] = json_style_vars;

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

    // Deserialize colors
    if (json_data.contains("colors"))
        {
        nlohmann::json json_colors = json_data["colors"];
        for (const auto& [key, value] : json_colors.items())
        {
            colors.insert_or_assign(static_cast<ImGuiCol>(std::stoi(key)), color::deserialize(value));
        }
    }

    // Deserialize style_vars
    if (json_data.contains("style_vars")) {
        nlohmann::json json_style_vars = json_data["style_vars"];
        for (const auto& [key, value] : json_style_vars.items())
        {
            style_vars.insert_or_assign(static_cast<ImGuiStyleVar>(std::stoi(key)), value.get<float>());
        }
    }
}
