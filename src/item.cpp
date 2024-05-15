#include "item.h"

#include "crafting_recipe.h"
#include "../minions/minion_calculator.h"


std::unordered_map<std::string, item> item::items;

nlohmann::json item::serialize() const
{
    nlohmann::json json_data;
    json_data["name"] = name;
    json_data["id"] = id;
    json_data["sell_price"] = sell_price;
    return json_data;
}

item item::deserialize(const nlohmann::json& json_data, nlohmann::json& bazaar_data)
{
    item item;
      
    item.name = json_data["name"];
    item.id = json_data["id"];
      
    if (json_data.contains("npc_sell_price"))
    {
        item.sell_price = json_data["npc_sell_price"];
    }
    else item.sell_price = 0;

    nlohmann::json& products = bazaar_data["products"][item.id];
    nlohmann::json& quick_status = products["quick_status"];
    if (quick_status != nullptr)
    {
        const double bazaar_sell_price = quick_status["sellPrice"];
        item.bazaar_sell_price = bazaar_sell_price;
    }
    
    for (const auto& element : minion_calculator::odd_items_ids)
    {
        if (item.id == element.first)
        {
            item.id = element.second;
        }
    }
    
    return item;
}

void item::print_item_info() const
{
    std::cout << "name: " << name << '\n';
    std::cout << "id: " << id << '\n';
    std::cout << "sell price: " << sell_price << '\n';
    std::cout << "bazaar sell price: " << bazaar_sell_price << '\n';
}



item item::get_enchanted_variant() const
{
    for (const auto& recipe: crafting_recipe::recipes)
    {
        if (recipe.second.item_id == id && recipe.second.craft_id.find("ENCHANTED") != std::string::npos)
        {
            //std::cout << items[recipe.second.craft_id].name << std::endl;
            return items[recipe.second.craft_id];
        }
    }
    return {};
}
