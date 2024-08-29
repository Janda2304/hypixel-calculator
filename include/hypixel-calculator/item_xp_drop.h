#ifndef ITEM_XP_DROP_H
#define ITEM_XP_DROP_H

#include <nlohmann/json.hpp>

enum skills
{
    mining,
    farming,
    foraging,
    fishing,
    combat,
    alchemy,
    enchanting,
    other
};

inline const char *to_string(skills e)
{
    switch (e)
    {
        case mining: return "mining";
        case farming: return "farming";
        case foraging: return "foraging";
        case fishing: return "fishing";
        case combat: return "combat";
        case alchemy: return "alchemy";
        case enchanting: return "enchanting";
        case other: return "other";
        default: return "unknown";
    }
}



struct item_xp_drop
{
    skills skill = skills::mining;
    float xp = 0;

    nlohmann::json serialize() const;

    static item_xp_drop deserialize(const nlohmann::json& json_data);
};



#endif //ITEM_XP_DROP_H


