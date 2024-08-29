#include <crafting_recipe.h>
#include <iostream>
#include <minion_calculator.h>
#include <item.h>
#include <minion.h>


std::map<std::string, crafting_recipe> crafting_recipe::recipes;

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

    for (const auto& element : minion_calculator::odd_items_ids)
    {
        if (recipe.craft_id == element.first)
        {
            recipe.craft_id = element.second;
        }
        if (recipe.item_id == element.first)
        {
            recipe.item_id = element.second;
        }
    }
    
    return recipe;
}

void crafting_recipe::print_recipe() const
{
    std::cout << "Crafting Recipe: " << craft_id << '\n';
    std::cout << "Item ID: " << item_id << '\n';
    std::cout << "Amount: " << item_amount << '\n';
}

std::string crafting_recipe::get_recipe_string() const
{
    return minion::minions[craft_id].name + '\n' + "Needed Item: " + item::items[item_id].name + '\n' + "Amount: " + std::to_string(item_amount - 1) + '\n';
}

void crafting_recipe::print_recipe_short() const
{
    std::cout << item_amount << "x " << item_id << '\n';
}
