#pragma once

#include "drop_data.h"
#include <nlohmann/json.hpp>

enum minion_type
{
    mining_minion,
    farming_minion,
    foraging_minion,
    fishing_minion,
    combat_minion,
    alchemy_minion,
    enchanting_minion,
    other_minion,
    slayer_minion
};

inline const char *to_string(minion_type e)
{
    switch (e)
    {
        case mining_minion: return "mining_minion";
        case farming_minion: return "farming_minion";
        case foraging_minion: return "foraging_minion";
        case fishing_minion: return "fishing_minion";
        case combat_minion: return "combat_minion";
        case alchemy_minion: return "alchemy_minion";
        case enchanting_minion: return "enchanting_minion";
        case other_minion: return "other_minion";
        case slayer_minion: return "slayer_minion";
        default: return "unknown";
    }
}




/**
 * \brief struct to represent a minion
 * \param type type of the minion
 * \param name name of the minion
 * \param tier tier of the minion
 * \param storage storage capacity of the minion
 * \param base_production_rate production rate of the minion in seconds (e.g 5 for every 5 seconds)
 * \param item_drop_rate item drop rate of the minion (e.g 4 for snow minion)
 * \param drop_item reference to the item that the minion drops
 */
struct minion
{
    minion_type type;
    std::string name;
    std::string id;
    int tier;
    int storage;
    float base_production_rate;
    std::vector<drop_data> drops;

    nlohmann::json serialize() const;

    static minion deserialize(const nlohmann::json& json_data);

    void print_minion_info() const;

    static std::map<std::string, minion> minions;

    void add_drop(const drop_data& drop);
};
