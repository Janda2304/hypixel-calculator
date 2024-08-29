#pragma once
#include <string>
#include <nlohmann/json.hpp>

struct crafting_recipe
{
    std::string craft_id;
    std::string item_id;
    int item_amount;

    nlohmann::json serialize() const;

    static crafting_recipe deserialize(const nlohmann::json& json_data);

    void print_recipe() const;

    std::string get_recipe_string() const;

    void print_recipe_short() const;

    static std::map<std::string, crafting_recipe> recipes;

    
    friend bool operator==(const crafting_recipe& lhs, const crafting_recipe& rhs)
    {
        return lhs.craft_id == rhs.craft_id &&
               lhs.item_id == rhs.item_id &&
               lhs.item_amount == rhs.item_amount;
    }

    friend bool operator!=(const crafting_recipe& lhs, const crafting_recipe& rhs)
    {
        return !(lhs == rhs);
    }
};
