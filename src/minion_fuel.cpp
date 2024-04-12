#include "minion_fuel.h"

#include <iostream>

std::map<std::string, minion_fuel> minion_fuel::minion_fuels;

nlohmann::json minion_fuel::serialize() const
{
    nlohmann::json json_data;
    json_data["name"] = name;
    json_data["fuel_time"] = fuel_time;
    json_data["effectivity_percentage"] = effectivity_percentage;
    json_data["multiplier"] = multiplier;
    return json_data;
}

minion_fuel minion_fuel::deserialize(const nlohmann::json& json_data)
{
    minion_fuel fuel;
    fuel.name = json_data["name"];
    fuel.id = json_data["id"];
    fuel.fuel_time = json_data["fuel_time"];
    fuel.effectivity_percentage = json_data["effectivity_percentage"];
    fuel.multiplier = json_data["multiplier"];
    return fuel;
}

void minion_fuel::print_fuel_info() const
{
    std::cout << "Fuel Name: " << name << '\n';
    std::cout << "Fuel Time: " << fuel_time << ", Effectivity Percentage: " << effectivity_percentage << ", Multiplier: " << multiplier << '\n';
}
