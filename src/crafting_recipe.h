﻿#pragma once
#include <string>
#include "../nlohmann/json.hpp"

struct crafting_recipe
{
    std::string craft_id;
    std::string item_id;
    int item_amount;

    nlohmann::json serialize() const;

    static crafting_recipe deserialize(const nlohmann::json& json_data);

    void print_recipe() const;

    void print_recipe_short() const;

    static std::map<std::string, crafting_recipe> recipes;
};
