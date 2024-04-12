#pragma once
#include <string>
#include "../nlohmann/json.hpp"


/**
 * \brief struct to represent a fuel for a minion
 * \param name name of the item
 * \param fuel_time fuel duration in seconds, -1 for infinite
 * \param effectivity_percentage effectivity of the fuel (e.g 25% for enchanted lava bucket)
 * \param multiplier if this fuel has a direct multiplier (e.g catalyst), then set this to the multiplier and effectivity_percentage to 0
 */
struct minion_fuel
{
    std::string name;
    std::string id;
    int fuel_time;
    int effectivity_percentage;
    int multiplier;

    nlohmann::json serialize() const;

    static minion_fuel deserialize(const nlohmann::json& json_data);

    void print_fuel_info() const;

    static std::map<std::string, minion_fuel> minion_fuels;
};
