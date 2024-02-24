#include "minion.h"

nlohmann::json minion::serialize() const
{
    nlohmann::json json_data;
    json_data["type"] = type;
    json_data["name"] = name;
    json_data["id"] = id;
    json_data["tier"] = tier;
    json_data["storage"] = storage;
    json_data["base_production_rate"] = base_production_rate;
    for (const auto& drop : drops)
    {
        json_data["drops"].push_back(drop.serialize());
    }
    return json_data;
}

minion minion::deserialize(const nlohmann::json& json_data)
{
    minion minion;
    minion.type = json_data["type"];
    minion.name = json_data["name"];
    minion.id = json_data["id"];
    minion.tier = json_data["tier"];
    minion.storage = json_data["storage"];
    minion.base_production_rate = json_data["base_production_rate"];
    for (const auto& drop : json_data["drops"])
    {
        minion.drops.push_back(drop_data::deserialize(drop));
    }
    return minion;
}

void minion::print_minion_info() const
{
    std::cout << "type: " << type << '\n';
    std::cout << "name: " << name << '\n';
    std::cout << "tier: " << tier << '\n';
    std::cout << "storage: " << storage << '\n';
    std::cout << "base production rate: " << base_production_rate << '\n';
    for (const auto& drop : drops)
    {
        std::cout << "item id: " << drop.item_id << '\n';
        std::cout << "drop rate: " << drop.drop_rate << '\n';
        std::cout << "drop chance: " << drop.drop_chance << '\n';
    }
}
