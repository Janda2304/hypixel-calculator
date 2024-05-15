#ifndef MINION_CALCULATION_DATA_H
#define MINION_CALCULATION_DATA_H
#include "crafting_recipe.h"
#include "drop_data.h"
#include "fuel_calculation_data.h"


struct minion_calculation_data
{
    float minion_production_rate;
    std::unordered_map<std::string, drop_data> minion_drops;
    std::map<std::string, float> drops_per_hour;
    std::map<std::string, float> profits_per_hour;
    std::map<std::string, float> bazaar_profit_per_hour;
    float sum_profit_npc;
    float sum_profit_bazaar;
    float storage_fill_up_time;
    float minion_return_ratio;
    fuel_calculation_data fuel_data;
    std::unordered_map<skills, float> skill_xp_drops;
    crafting_recipe minion_recipe;
    
};



#endif //MINION_CALCULATION_DATA_H
