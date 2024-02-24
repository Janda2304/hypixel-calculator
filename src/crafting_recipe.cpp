#include "crafting_recipe.h"

#include <iostream>

#include "../nlohmann/json.hpp"

nlohmann::json crafting_recipe::serialize() const
{
    nlohmann::json json_data;
    json_data["craft_id"] = craft_id;
    json_data["item_id"] = item_id;
    json_data["item_amount"] = item_amount;
    return json_data;
}

crafting_recipe crafting_recipe::deserialize(const nlohmann::json& json_data)
{
    crafting_recipe recipe;
    recipe.craft_id = json_data["craft_id"];
    recipe.item_id = json_data["item_id"];
    recipe.item_amount = json_data["item_amount"];
    return recipe;
}

void crafting_recipe::print_recipe() const
{
    std::cout << "Crafting Recipe: " << craft_id << '\n';
    std::cout << "Item ID: " << item_id << '\n';
    std::cout << "Amount: " << item_amount << '\n';
}

void crafting_recipe::print_recipe_short() const
{
    std::cout << item_amount << "x " << item_id << '\n';
}
