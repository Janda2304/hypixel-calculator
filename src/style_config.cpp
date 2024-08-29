#include "style_config.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <imgui.h>  // Ensure this header is included if it's not already

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

    for (const auto& style_pair : style_vars)
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

    for (auto it = colors.begin(); it != colors.end(); ++it)
    {
        color c(it->second);
        json_colors[std::to_string(it->first)] = c.serialize();
    }

    for (auto it = style_vars.begin(); it != style_vars.end(); ++it)
    {
        json_style_vars[std::to_string(it->first)] = it->second;
    }

    json_data["colors"] = json_colors;
    json_data["style_vars"] = json_style_vars;

    std::ofstream file(path);
    if (file.is_open())
    {
        file << json_data.dump(4);
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file for writing: " << path << std::endl;
    }
}

void style_config::deserialize(const std::string &path)
{
    nlohmann::json json_data;
    std::ifstream file(path);
    if (file.is_open())
    {
        file >> json_data;
        file.close();

        // Deserialize colors
        if (json_data.contains("colors"))
        {
            nlohmann::json json_colors = json_data["colors"];
            for (auto it = json_colors.items().begin(); it != json_colors.items().end(); ++it)
            {
                colors.insert_or_assign(static_cast<ImGuiCol>(std::stoi(it.key())), color::deserialize(it.value()));
            }
        }

        // Deserialize style_vars
        if (json_data.contains("style_vars"))
        {
            nlohmann::json json_style_vars = json_data["style_vars"];
            for (auto it = json_style_vars.items().begin(); it != json_style_vars.items().end(); ++it)
            {
                style_vars.insert_or_assign(static_cast<ImGuiStyleVar>(std::stoi(it.key())), it.value().get<float>());
            }
        }
    }
    else
    {
        std::cerr << "Unable to open file for reading: " << path << std::endl;
    }
}