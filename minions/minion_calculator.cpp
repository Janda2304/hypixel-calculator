#include "minion_calculator.h"
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>


#include "../helper.hpp"
#include "../src/crafting_recipe.h"
#include "../src/drop_data.h"
#include "../src/item.h"
#include "../src/minion_fuel.h"
#include "../src/buy_requirements.h"
#include "../src/fuel_calculation_data.h"
#include "../src/minion_calculation_data.h"


bool minion_calculator::diamond_spreading = false;
std::string minion_calculator::selected_minion_id;
int minion_calculator::selected_minion_index;
int minion_calculator::storage_capacity = 0;
int minion_calculator::other_boosts_percentage = 0;

std::string minion_calculator::selected_fuel_id;
int minion_calculator::selected_fuel_index;

std::map<std::string, std::string> minion_calculator::odd_items_ids
{
    {"SLIME_BALL", "SLIMEBALL"},
    {"RAW_FISH:1", "RAW_SALMON"},
    {"CLAY_BALL", "CLAY"},
    {"MITHRIL_ORE", "MITHRIL"},
    {"MYCEL", "MYCELIUM"},
    {"LOG", "OAK_WOOD"},
    {"LOG:1", "SPRUCE_WOOD"},
    {"LOG:2", "BIRCH_WOOD"},
    {"LOG:3", "JUNGLE_WOOD"},
    {"LOG_2", "ACACIA_WOOD"},
    {"LOG_2:1", "DARK_OAK_WOOD"},
    {"SULPHUR", "GUNPOWDER"},
    {"RED_ROSE", "FLOWER"},
    {"ENDER_STONE", "END_STONE"},
    {"INK_SACK:3", "COCOA_BEANS"},
    {"INK_SACK:4", "LAPIS_LAZULI"},
    {"CARROT_ITEM", "CARROT"},
    {"POTATO_ITEM", "POTATO"},
    {"CHEESE_FUEL", "TASTY_CHEESE"},
    {"NETHER_STALK", "NETHER_WART"},
    {"SNOW_BALL", "SNOWBALL"},
    {"RAW_FISH:3", "PUFFERFISH"},
    {"RAW_FISH:2", "CLOWNFISH"}
};


void minion_calculator::save_fuels(const std::string &path)
{
    nlohmann::json json_data;
    for (const auto &fuel: minion_fuel::minion_fuels)
    {
        json_data[fuel.first] = fuel.second.serialize();
    }
    std::ofstream output_file(path);
    output_file << json_data.dump(4);
    output_file.close();
}

void minion_calculator::save_items(const std::string &path)
{
    nlohmann::json json_data;
    for (const auto &item: item::items)
    {
        json_data[item.first] = item.second.serialize();
    }

    std::ofstream output_file(path);
    output_file << json_data.dump(4);
    output_file.close();
}

void minion_calculator::save_minions(const std::string &path)
{
    nlohmann::json json_data;
    for (const auto &minion: minion::minions)
    {
        json_data[minion.first] = minion.second.serialize();
    }

    std::ofstream output_file(path);
    output_file << json_data.dump(4);
    output_file.close();
}


void minion_calculator::init_fuels(const std::string &path)
{
    std::ifstream input_file(path);
    nlohmann::json json_data;
    input_file >> json_data;
    input_file.close();

    for (const auto &fuel_data: json_data)
    {
        minion_fuel fuel = minion_fuel::deserialize(fuel_data);
        minion_fuel::minion_fuels.insert({fuel.id, fuel});
    }
}

void minion_calculator::init_items(const std::string &path)
{
    std::string bazaar_string = helper::exec("curl -k -X GET \"https://api.hypixel.net/v2/skyblock/bazaar\"");
    nlohmann::json bazaar_data;
    if (!bazaar_string.empty())
    {
        bazaar_data = nlohmann::json::parse(bazaar_string);
    }

    std::ifstream input_file(path);
    nlohmann::json json_data;
    input_file >> json_data;
    input_file.close();

    for (const auto &item_data: json_data)
    {
        item itm = item::deserialize(item_data, bazaar_data);
        item::items.insert({itm.id, itm});
    }
}

void minion_calculator::init_minions(const std::string &path)
{
    std::ifstream input_file(path);
    nlohmann::json json_data;
    input_file >> json_data;
    input_file.close();
    for (const auto &minion_data: json_data)
    {
        minion minion = minion::deserialize(minion_data);
        minion::minions.insert({minion.id, minion});
    }
}

void minion_calculator::init_recipes(const std::string &path)
{
    std::ifstream input_file(path);
    nlohmann::json json_data;
    input_file >> json_data;
    input_file.close();
    for (const auto &recipe_data: json_data)
    {
        crafting_recipe recipe = crafting_recipe::deserialize(recipe_data);
        crafting_recipe::recipes.insert({recipe.craft_id, recipe});
    }
}

void minion_calculator::init_buy_requirements(const std::string &path)
{
    std::ifstream input_file(path);
    nlohmann::json json_data;
    input_file >> json_data;
    input_file.close();
    for (const auto &buy_requirements_data: json_data)
    {
        item_buy_requirements requirements = item_buy_requirements::deserialize(buy_requirements_data);
        item_buy_requirements::buy_requirements.insert({requirements.offer_id, requirements});
    }
}



void minion_calculator::calc_minion_profit(minion minion, minion_calculation_data& calculation_data, const minion_fuel& fuel, bool diamond_spreading)
{
    float production_rate_boost = 0;
    float multiplier = 1;
    int storage_boost = 0;
    std::unordered_map<std::string, drop_data> minion_drops;
    fuel_calculation_data fuel_calculation_data{};
    std::unordered_map<skills, float> skill_xp_drops;

    for (const auto& minion_drop: minion.drops)
    {
        minion_drops.insert({minion_drop.item_id, minion_drop});
    }
    
    if (fuel.effectivity_percentage > 0)
    {
        production_rate_boost += static_cast<float>(fuel.effectivity_percentage);
    }
    else if (fuel.multiplier > 0)
    {
        multiplier = static_cast<float>(fuel.multiplier);
    }
    minion.storage += storage_boost * 64;

    if (diamond_spreading && minion.id.find("DIAMOND") == std::string::npos)
    {
        //TODO: rewrite this, diamond spreading works differently (it gives a diamond every 10th item drop not minion action)
        drop_data diamond;
        diamond.item_id = "DIAMOND";
        diamond.drop_chance = 10;
        diamond.drop_rate = 1;
        diamond.xp_drop.skill = skills::mining;
        diamond.xp_drop.xp = 0.4f;
        minion_drops.insert({"DIAMOND", diamond});
    }
    else if (diamond_spreading && minion.id.find("DIAMOND") != std::string::npos)
    {
        //if the minion is a diamond minion and diamond spreading is used, the production rate is increased by 10%
        production_rate_boost += 10;
    }

    if (!diamond_spreading && minion.id.find("DIAMOND") == std::string::npos)
    {
        minion_drops.erase("DIAMOND");
    }

    
    
    float production_rate = -1;
    float actions_per_hour;
    float drops_per_action;
    float production_rate_multiplier = 2;
    std::map<std::string, float> drops_per_hour;
    std::map<std::string, float> profits_per_hour;
    std::map<std::string, float> bazaar_profit_per_hour;
    float sum_profit = 0;
    float bazaar_sum_profit = 0;

    if (minion.id.find("FISHING") != std::string::npos)
    {
        production_rate_multiplier = 1;
    }
    production_rate_boost += static_cast<float>(other_boosts_percentage);
    
    for (const auto &drop: minion_drops)
    {
        item item = item::items[drop.second.item_id];
        
        production_rate = minion.base_production_rate / (1 + production_rate_boost * 0.01);
        
        actions_per_hour = 3600 / (production_rate * production_rate_multiplier); 

        //how much of this item drops per one action
        drops_per_action = drop.second.drop_rate * (drop.second.drop_chance * 0.01f); 

        //how much of this item is generated per hour
        drops_per_hour.insert({item.id, actions_per_hour * drops_per_action * multiplier}); 
        
        profits_per_hour.insert({item.id, drops_per_hour[item.id] * item.sell_price});
        
        bazaar_profit_per_hour.insert({item.id, drops_per_hour[item.id] * item.bazaar_sell_price});
    }


    float drops_per_hour_sum = 0;
    
    //TODO: out crafting_recipe_data
    
    
    for (const auto &id: drops_per_hour)
    {
        
        drop_data drop = minion_drops[id.first];
        item item = item::items[id.first];

        float profit = profits_per_hour[item.id];
        float bazaar_profit = bazaar_profit_per_hour[item.id];
        
        sum_profit += profit;
        bazaar_sum_profit += bazaar_profit;
        drops_per_hour_sum += id.second;
        
        skill_xp_drops[drop.xp_drop.skill] += drop.xp_drop.xp * id.second;
    }
    float fill_up_time = static_cast<float>(minion.storage + (storage_capacity * 64)) / drops_per_hour_sum; //hours
    
    
   // std::cout << "Minion crafting recipe: " << '\n';

    crafting_recipe minion_recipe = crafting_recipe::recipes[minion.id];
    item_buy_requirements requirement = item_buy_requirements::buy_requirements[minion.id];


    float base_item_drops = 0;
    if (!minion_recipe.craft_id.empty())
    {
        //minion_recipe.print_recipe_short();
        base_item_drops = drops_per_hour[minion_recipe.item_id];
    }


    if (!requirement.offer_id.empty())
    {
        //requirement.print_buy_requirements_short();

        //not needed because t12 never wants the base item, but just in case they change something
        base_item_drops = drops_per_hour[requirement.item_id];
    }

    crafting_recipe minion_item_recipe;
    crafting_recipe enchanted_item_recipe;
    crafting_recipe enchanted_item_recipe_2;
    int base_item_needed_amount = 0;
    
    float return_ratio;
    
    if (base_item_drops > 0)
    {
        return_ratio = static_cast<float>(minion_recipe.item_amount) / base_item_drops;
       // std::cout << "The minion returns the resource investment in " << return_ratio << " hours" << '\n';
    }
    else if (base_item_drops <= 0 && !minion_recipe.craft_id.empty())
    {
        minion_item_recipe = crafting_recipe::recipes[minion_recipe.item_id];
        base_item_needed_amount = 0;
        if (drops_per_hour.find(minion_item_recipe.item_id) == drops_per_hour.end())
        {
            enchanted_item_recipe_2 = minion_item_recipe;

            enchanted_item_recipe = crafting_recipe::recipes[enchanted_item_recipe_2.item_id];

            int enchanted_item_needed_amount = enchanted_item_recipe_2.item_amount * minion_recipe.item_amount;
            base_item_needed_amount = enchanted_item_needed_amount * enchanted_item_recipe.item_amount;
           // std::cout << enchanted_item_needed_amount << "x " << enchanted_item_recipe_2.item_id << '\n';
        }
        else
        {
            enchanted_item_recipe = minion_item_recipe;
            base_item_needed_amount = enchanted_item_recipe.item_amount * minion_recipe.item_amount;
        }

       // std::cout << base_item_needed_amount << "x " << enchanted_item_recipe.item_id << '\n';
        return_ratio = static_cast<float>(base_item_needed_amount) / drops_per_hour[enchanted_item_recipe.item_id];
    }
    else
    {
        minion_item_recipe = crafting_recipe::recipes[item_buy_requirements::buy_requirements[minion.id].item_id];
        if (drops_per_hour.find(minion_item_recipe.item_id) == drops_per_hour.end())
        {
            enchanted_item_recipe_2 = minion_item_recipe;

            enchanted_item_recipe = crafting_recipe::recipes[enchanted_item_recipe_2.item_id];

            int enchanted_item_needed_amount = enchanted_item_recipe_2.item_amount * requirement.item_amount;
            base_item_needed_amount = enchanted_item_needed_amount * enchanted_item_recipe.item_amount;
           // std::cout << enchanted_item_needed_amount << "x " << enchanted_item_recipe_2.item_id << '\n';
        }
        else
        {
            enchanted_item_recipe = minion_item_recipe;
            base_item_needed_amount = enchanted_item_recipe.item_amount * requirement.item_amount;
        }
        //std::cout << base_item_needed_amount << "x " << enchanted_item_recipe.item_id << '\n';
        if (drops_per_hour.find(enchanted_item_recipe.item_id) != drops_per_hour.end())
        {
            return_ratio = static_cast<float>(base_item_needed_amount) / drops_per_hour[enchanted_item_recipe.item_id];
        }
        else
        {
           // std::cout << "The minion don't produce the resources used to craft it's tier\n";
            return;
        }
    }


    /*if (minion.tier > 1)
    {
        std::stringstream ss(minion.id);
        std::string id;
        std::vector<std::string> ids;
        while (std::getline(ss, id, '_'))
        {
            ids.push_back(id);
        }

        std::string tier_id = ids[0] + "_" + ids[1] + "_" + std::to_string(minion.tier - 1);
        std::cout << tier_id << '\n';
    }*/
    
    
    float needed_fuel = 86400.0f / static_cast<float>(fuel.fuel_time);
    fuel_calculation_data.fuel_duration = fuel.fuel_time / 3600.0f;
    fuel_calculation_data.fuel_cost_per_day = needed_fuel * item::items[fuel.id].bazaar_sell_price;
    fuel_calculation_data.needed_fuel_per_day = needed_fuel;
    calculation_data.fuel_data = fuel_calculation_data;
    calculation_data.minion_drops = minion_drops;
    calculation_data.drops_per_hour = drops_per_hour;
    calculation_data.profits_per_hour = profits_per_hour;
    calculation_data.bazaar_profit_per_hour = bazaar_profit_per_hour;
    calculation_data.sum_profit_npc = sum_profit;
    calculation_data.sum_profit_bazaar = bazaar_sum_profit;
    calculation_data.storage_fill_up_time = fill_up_time;
    calculation_data.minion_return_ratio = return_ratio;
    calculation_data.skill_xp_drops = skill_xp_drops;
    calculation_data.minion_production_rate = production_rate;
}

/*void minion_calculator::calc_best_minion()
{
    using minion_profit = std::pair<minion, float>;
    auto comp = [](const minion_profit &a, const minion_profit &b) { return a.second > b.second; };
    std::priority_queue<minion_profit, std::vector<minion_profit>, decltype(comp)> top_minions(comp);
    std::priority_queue<minion_profit, std::vector<minion_profit>, decltype(comp)> top_minions_bazaar(comp);

    minion_fuel fuel;
    std::string fuel_id;
    float production_rate_boost = 0;
    bool diamond_spreading = false;
    int storage_boost;

    calculate_minion_boosts(fuel, production_rate_boost, storage_boost, diamond_spreading);
    
    for (auto minion_pair: minion::minions)
    {
        minion_pair.second.storage += storage_boost * 64;
        minion &out = minion_pair.second;
        float profit;
        float bazaar_profit;
        calc_minion_profit(out, profit, bazaar_profit, fuel, diamond_spreading, false, false);
        top_minions.emplace(out, profit);
        top_minions_bazaar.emplace(out, bazaar_profit);
        if (top_minions.size() > 3)
        {
            top_minions.pop();
        }
        if (top_minions_bazaar.size() > 3)
        {
            top_minions_bazaar.pop();
        }
    }
    helper::clear();
    std::cout << "NPC SELL PROFIT: " << '\n';

    while (!top_minions.empty())
    {
        minion m = top_minions.top().first;
        float p = top_minions.top().second;
        float pb = top_minions.top().second;
        calc_minion_profit(m, p, pb, fuel, diamond_spreading, false, true);
        top_minions.pop();
        std::cout << '\n';
    }

    std::cout << "BAZAAR SELL PROFIT: " << '\n';

    while (!top_minions_bazaar.empty())
    {
        minion m = top_minions_bazaar.top().first;
        float p = top_minions_bazaar.top().second;
        float pb = top_minions_bazaar.top().second;
        calc_minion_profit(m, p, pb, fuel, diamond_spreading, false, true);
        top_minions_bazaar.pop();
        std::cout << '\n';
    }
}*/
