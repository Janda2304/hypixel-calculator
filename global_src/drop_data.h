#pragma once
#include <string>
#include "../single_include/nlohmann/json.hpp"

struct drop_data
{
    std::string item_id;
    float drop_rate;
    float drop_chance;

    nlohmann::json serialize() const;

    static drop_data deserialize(const nlohmann::json& json_data);
};
