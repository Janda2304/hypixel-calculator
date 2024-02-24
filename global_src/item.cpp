#include "item.h"

#include "../minion_calculator.h"


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
    
    for (auto element : minion_calculator::odd_items_ids)
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