

#include "item_xp_drop.h"

nlohmann::json item_xp_drop::serialize() const
{
    nlohmann::json json_data;
    json_data["skill"] = skill;
    json_data["xp"] = xp;
    return json_data;
    
}

item_xp_drop item_xp_drop::deserialize(const nlohmann::json &json_data)
{
    item_xp_drop data;
    data.skill = json_data["skill"];
    data.xp = json_data["xp"];
    return data;
}
