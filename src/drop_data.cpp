#include <drop_data.h>

#include <nlohmann/json.hpp>


nlohmann::json drop_data::serialize() const
{
    nlohmann::json json_data;
    json_data["item_id"] = item_id;
    json_data["drop_rate"] = drop_rate;
    json_data["drop_chance"] = drop_chance;
    json_data["xp_drop"] = xp_drop.serialize();
    return json_data;
}

drop_data drop_data::deserialize(const nlohmann::json& json_data)
{
    drop_data data;
    data.item_id = json_data["item_id"];
    data.drop_rate = json_data["drop_rate"];
    data.drop_chance = json_data["drop_chance"];
    data.xp_drop = item_xp_drop::deserialize(json_data["xp_per_item"]);
    return data;
}
