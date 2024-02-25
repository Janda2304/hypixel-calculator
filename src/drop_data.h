#pragma once
#include <string>
#include "../nlohmann/json.hpp"

struct drop_data
{
    std::string item_id;
    float drop_rate;
    float drop_chance;

    nlohmann::json serialize() const;

    static drop_data deserialize(const nlohmann::json& json_data);

    bool operator==(const drop_data& rhs) const
    {
        return item_id == rhs.item_id && drop_rate == rhs.drop_rate && drop_chance == rhs.drop_chance;
    }
};
