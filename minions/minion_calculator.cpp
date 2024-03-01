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
    {"CHEESE_FUEL", "TASTY_CHEESE"}
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


void calculate_minion_boost(minion_fuel &fuel, float &production_rate_boost, int &storage_boost, bool &diamond_spreading)
{
    //fuel
    int i = 0;
    for (const auto &fuel_pair: minion_fuel::minion_fuels)
    {
        std::cout << i << ")" << fuel_pair.first << '\n';
        fuel_pair.second.print_fuel_info();
        i++;
    }

    std::string fuel_id;
    std::cout << "Enter the fuel id:\n";
    std::cin >> fuel_id;
    fuel = minion_fuel::minion_fuels[fuel_id];
    //

    //diamond spreading
    std::cout << "Do you use diamond spreading? (y/n)\n";
    char choice;
    std::cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        diamond_spreading = true;
    }
    //

    //storage
    std::cout << "Do your minion have storage? Enter the amount of storage slots or 0 if it doesn't have any\n";
    std::cin >> storage_boost;
    //


    //other means of production rate boost
    std::cout << "Enter the sum of production rate boost added by other means (in %)" << '\n';
    int boost;
    std::cin >> boost;
    production_rate_boost += boost;
    helper::clear();
    //
}


void minion_calculator::calc_minion_profit(minion &minion, float &profit_out, float &bazaar_profit_out,
                                           minion_fuel &fuel, bool diamond_spreading, bool manual_calculation,
                                           bool print_result)
{
    float production_rate_boost = 0;
    float multiplier = 1;
    int storage_boost = 0;
    std::cout << std::setprecision(3);

    if (manual_calculation)
    {
        calculate_minion_boost(fuel, production_rate_boost, storage_boost, diamond_spreading);
    }

    if (fuel.effectivity_percentage > 0)
    {
        production_rate_boost += static_cast<float>(fuel.effectivity_percentage);
    } else if (fuel.multiplier > 0)
    {
        multiplier = static_cast<float>(fuel.multiplier);
    }
    minion.storage += storage_boost * 64;

    if (diamond_spreading && minion.id.find("DIAMOND") == std::string::npos)
    {
        drop_data diamond;
        diamond.item_id = "DIAMOND";
        diamond.drop_chance = 10;
        diamond.drop_rate = 1;
        diamond.xp_drop.skill = skills::mining;
        diamond.xp_drop.xp = 0.4f;
        minion.add_drop(diamond);
    } else if (diamond_spreading && minion.id.find("DIAMOND") != std::string::npos)
    {
        //if the minion is a diamond minion and diamond spreading is used, the production rate is increased by 10%
        production_rate_boost += 10;
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
    int index = 0;

    if (minion.id.find("FISHING") != std::string::npos)
    {
        production_rate_multiplier = 1;
    }

    for (const auto &minion_drop: minion.drops)
    {
        item item = item::items[minion_drop.item_id];
        production_rate = minion.base_production_rate / (1 + production_rate_boost * 0.01);
        actions_per_hour = 3600 / (production_rate * production_rate_multiplier);
        drops_per_action = minion.drops[index].drop_rate * (minion.drops[index].drop_chance * 0.01f);
        drops_per_hour.insert({item.id, actions_per_hour * drops_per_action * multiplier});
        profits_per_hour.insert({item.id, drops_per_hour[item.id] * item.sell_price});
        bazaar_profit_per_hour.insert({item.id, drops_per_hour[item.id] * item.bazaar_sell_price});
        index++;
    }


    float drops_per_hour_sum = 0;
    if (print_result)
    {
        std::cout << std::defaultfloat;
        std::cout << "Minion: " << minion.name << '\n';
        std::cout << "Production rate: " << production_rate << '\n' << '\n';
    }

    int drop_index = 0;
    for (const auto &id: drops_per_hour)
    {
        sum_profit += profits_per_hour[item::items[id.first].id];
        bazaar_sum_profit += bazaar_profit_per_hour[item::items[id.first].id];
        drops_per_hour_sum += id.second;

        if (print_result)
        {
            std::cout << item::items[id.first].name << ":\n";
            std::cout << id.second << " per hour" << '\n';

            std::cout << std::fixed;
            std::cout << profits_per_hour[item::items[id.first].id] << " coins per hour" << '\n';
            std::cout << bazaar_profit_per_hour[item::items[id.first].id] << " coins per hour (bazaar)" << '\n';
            std::cout << "You will get " << drops_per_hour[minion.drops[drop_index].item_id] * minion.drops[drop_index].xp_drop.xp << ' ' << to_string(minion.
               drops[drop_index].xp_drop.skill) << " xp per hour" << '\n' << '\n';
            drop_index++;
        }
    }
    profit_out = sum_profit;
    bazaar_profit_out = bazaar_sum_profit;
    if (!print_result) return;
    std::cout << "Minion will fill up in " << minion.storage / drops_per_hour_sum << " hours (storage capacity: " <<
            minion.storage << ')' << '\n';
    if (fuel.fuel_time > 0)
    {
        float needed_fuel = 86400.0f / static_cast<float>(fuel.fuel_time);
        std::cout << "Fuel will last " << fuel.fuel_time / 3600.0f << " hours" << '\n';
        std::cout << "You will need " << needed_fuel << " fuel per day" << '\n';
        std::cout << "This fuel will cost approximately " << needed_fuel * item::items[fuel.id].bazaar_sell_price <<
                " coins per day if bought at the bazaar" << '\n';
    } else
    {
        std::cout << "Fuel will last forever" << '\n';
    }

    std::cout << "Total profit per hour: " << sum_profit << '\n';
    std::cout << "Total profit per day: " << sum_profit * 24 << '\n' << '\n';

    std::cout << std::fixed << "Total profit per hour (bazaar): " << bazaar_sum_profit << '\n';
    std::cout << std::fixed << "Total profit per day (bazaar): " << bazaar_sum_profit * 24 << '\n' << '\n';

    std::cout << "Minion crafting recipe: " << '\n';

    crafting_recipe minion_recipe = crafting_recipe::recipes[minion.id];
    item_buy_requirements requirement = item_buy_requirements::buy_requirements[minion.id];


    float base_item_drops = 0;
    if (!minion_recipe.craft_id.empty())
    {
        minion_recipe.print_recipe_short();
        base_item_drops = drops_per_hour[minion_recipe.item_id];
    }


    if (!requirement.offer_id.empty())
    {
        requirement.print_buy_requirements_short();

        //not needed because t12 never wants the base item, but just in case they change something
        base_item_drops = drops_per_hour[requirement.item_id];
    }

    crafting_recipe minion_item_recipe;
    crafting_recipe enchanted_item_recipe;
    crafting_recipe enchanted_item_recipe_2;
    int base_item_needed_amount = 0;

    if (base_item_drops > 0)
    {
        float return_ratio = static_cast<float>(minion_recipe.item_amount) / base_item_drops;
        std::cout << "The minion returns the resource investment in " << return_ratio << " hours" << '\n';
    }
    else if (base_item_drops <= 0 && !minion_recipe.craft_id.empty())
    {
        crafting_recipe minion_item_recipe = crafting_recipe::recipes[minion_recipe.item_id];
        crafting_recipe enchanted_item_recipe;
        crafting_recipe enchanted_item_recipe_2;
        int base_item_needed_amount = 0;
        if (drops_per_hour.find(minion_item_recipe.item_id) == drops_per_hour.end())
        {
            enchanted_item_recipe_2 = minion_item_recipe;

            enchanted_item_recipe = crafting_recipe::recipes[enchanted_item_recipe_2.item_id];

            int enchanted_item_needed_amount = enchanted_item_recipe_2.item_amount * minion_recipe.item_amount;
            base_item_needed_amount = enchanted_item_needed_amount * enchanted_item_recipe.item_amount;
            std::cout << enchanted_item_needed_amount << "x " << enchanted_item_recipe_2.item_id << '\n';
        }
        else
        {
            enchanted_item_recipe = minion_item_recipe;
            base_item_needed_amount = enchanted_item_recipe.item_amount * minion_recipe.item_amount;
        }

        std::cout << base_item_needed_amount << "x " << enchanted_item_recipe.item_id << '\n';
        float return_ratio = static_cast<float>(base_item_needed_amount) / drops_per_hour[enchanted_item_recipe.
                                 item_id];
        std::cout << "The minion returns the resource investment for it's tier in " << return_ratio << " hours (or " <<
                return_ratio / 24 << " days)" << '\n';
    }
    else
    {
        crafting_recipe minion_item_recipe = crafting_recipe::recipes[item_buy_requirements::buy_requirements[minion.id].item_id];
        crafting_recipe enchanted_item_recipe;
        crafting_recipe enchanted_item_recipe_2;
        int base_item_needed_amount = 0;
        if (drops_per_hour.find(minion_item_recipe.item_id) == drops_per_hour.end())
        {
            enchanted_item_recipe_2 = minion_item_recipe;

            enchanted_item_recipe = crafting_recipe::recipes[enchanted_item_recipe_2.item_id];

            int enchanted_item_needed_amount = enchanted_item_recipe_2.item_amount * requirement.item_amount;
            base_item_needed_amount = enchanted_item_needed_amount * enchanted_item_recipe.item_amount;
            std::cout << enchanted_item_needed_amount << "x " << enchanted_item_recipe_2.item_id << '\n';
        }
        else
        {
            enchanted_item_recipe = minion_item_recipe;
            base_item_needed_amount = enchanted_item_recipe.item_amount * requirement.item_amount;
        }
        std::cout << base_item_needed_amount << "x " << enchanted_item_recipe.item_id << '\n';
        float return_ratio;
        if (drops_per_hour.find(enchanted_item_recipe.item_id) != drops_per_hour.end())
        {
            return_ratio = static_cast<float>(base_item_needed_amount) / drops_per_hour[enchanted_item_recipe.item_id];
        }
        else
        {
            std::cout << "The minion don't produce the resources used to craft it's tier\n";
            return;
        }
        std::cout << "The minion returns the resource investment for it's tier in " << return_ratio << " hours (or " << return_ratio / 24 << " days)" << '\n';
    }


    if (minion.tier > 1)
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
    }

    std::cout << "-----------------------------------" << '\n';
}

void minion_calculator::calc_best_minion()
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

    calculate_minion_boost(fuel, production_rate_boost, storage_boost, diamond_spreading);
    
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
}
