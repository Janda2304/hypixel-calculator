#pragma once
#include <string>

#include <item_xp_drop.h>
#include "nlohmann/json.hpp"

struct drop_data
{
    std::string item_id;
    float drop_rate;
    float drop_chance;
    item_xp_drop xp_drop;

    nlohmann::json serialize() const;

    static drop_data deserialize(const nlohmann::json& json_data);

    bool operator==(const drop_data& rhs) const
    {
        return item_id == rhs.item_id && drop_rate == rhs.drop_rate && drop_chance == rhs.drop_chance;
    }
};
