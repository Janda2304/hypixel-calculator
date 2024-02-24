#include "minion_calculator.h"
#include <map>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>

#include "helper.h"
#include "global_src/item.h"
#include "global_src/drop_data.h"
#include "global_src/minion_fuel.h"
#include "global_src/crafting_recipe.h"


std::unordered_map<std::string, item> minion_calculator::items;

std::map<std::string, minion> minion_calculator::minions;

std::map<std::string, minion_fuel> minion_calculator::minion_fuels;

std::map<std::string, crafting_recipe> minion_calculator::recipes;

std::map<std::string, item_buy_requirements> minion_calculator::buy_requirements;

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


void minion_calculator::save_fuels(const std::string& path)
{
}

void minion_calculator::save_items(const std::string& path)
{
    nlohmann::json json_data;
    for (const auto& item : items)
    {
        json_data[item.first] = item.second.serialize();
    }

    std::ofstream output_file(path);
    output_file << json_data.dump(4);
    output_file.close();
}

void minion_calculator::save_minions(const std::string& path)
{
    nlohmann::json json_data;
    for (const auto& minion : minions)
    {
        json_data[minion.first] = minion.second.serialize();
    }

    std::ofstream output_file(path);
    output_file << json_data.dump(4);
    output_file.close();
}


void minion_calculator::init_fuels(const std::string& path)
{
    // Deserialize JSON array into a vector of minion_fuel
    std::ifstream input_file(path);
    nlohmann::json json_data;
    input_file >> json_data;
    input_file.close();

    for (const auto& fuel_data : json_data)
    {
        minion_fuel fuel = minion_fuel::deserialize(fuel_data);
        minion_fuels.insert({fuel.id, fuel});
    }
}

void minion_calculator::init_items(const std::string& path)
{
    std::string bazaar_string = helper::exec("curl -k -X GET \"https://api.hypixel.net/v2/skyblock/bazaar\"");
    nlohmann::json bazaar_data = nlohmann::json::parse(bazaar_string);
    std::ifstream input_file(path);
    nlohmann::json json_data;
    input_file >> json_data;
    input_file.close();

    for (const auto& item_data : json_data)
    {
        item itm = item::deserialize(item_data, bazaar_data);
        items.insert({itm.id, itm});
    }
}

void minion_calculator::init_minions(const std::string& path)
{
    std::ifstream input_file(path);
    nlohmann::json json_data;
    input_file >> json_data;
    input_file.close();
    for (auto minion_data : json_data)
    {
        minion minion = minion::deserialize(minion_data);
        minions.insert({minion.id, minion});
    }
}

void minion_calculator::init_recipes(const std::string& path)
{
    std::ifstream input_file(path);
    nlohmann::json json_data;
    input_file >> json_data;
    input_file.close();
    for (const auto& recipe_data : json_data)
    {
        crafting_recipe recipe = crafting_recipe::deserialize(recipe_data);
        recipes.insert({recipe.craft_id, recipe});
    }
}

void minion_calculator::init_buy_requirements(const std::string& path)
{
    std::ifstream input_file(path);
    nlohmann::json json_data;
    input_file >> json_data;
    input_file.close();
    for (const auto& buy_requirements_data : json_data)
    {
        item_buy_requirements requirements = item_buy_requirements::deserialize(buy_requirements_data);
        buy_requirements.insert({requirements.offer_id, requirements});
    }
}


void manual_calc(minion_fuel& fuel, float& production_rate_boost, int& storage_boost, bool& diamond_spreading)
{
    //fuel
    int i = 0;
    for (const auto& fuel_pair : minion_calculator::minion_fuels)
    {
        std::cout << i << ")" << fuel_pair.first << '\n';
        fuel_pair.second.print_fuel_info();
        i++;
    }

    std::string fuel_id;
    std::cout << "Enter the fuel id:\n";
    std::cin >> fuel_id;
    fuel = minion_calculator::minion_fuels[fuel_id];
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
    //
}


void minion_calculator::calc_minion_profit(minion& minion, float& profit_out, float& bazaar_profit_out,
                                           minion_fuel& fuel, bool diamond_spreading, bool manual_calculation,
                                           bool print_result)
{
    std::vector<item> drop_items;
    float production_rate_boost = 0;
    float multiplier = 1;
    int storage_boost = 0;

    drop_items.reserve(minion.drops.size());
    for (const auto& id : minion.drops)
    {
        drop_items.push_back(minion_calculator::items[id.item_id]);
    }

    if (manual_calculation)
    {
        manual_calc(fuel, production_rate_boost, storage_boost, diamond_spreading);
        helper::clear();
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
        drop_items.push_back(minion_calculator::items["DIAMOND"]);
        drop_data diamond;
        diamond.item_id = "DIAMOND";
        diamond.drop_chance = 10;
        diamond.drop_rate = 1;
        minion.drops.push_back(diamond);
    }
    else if (diamond_spreading && minion.id.find("DIAMOND") != std::string::npos)
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

    for (const auto& item : drop_items)
    {
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
        std::cout << "Minion: " << minion.name << '\n';
        std::cout << "Production rate: " << production_rate << '\n' << '\n';
    }
    for (const auto& id : drops_per_hour)
    {
        sum_profit += profits_per_hour[minion_calculator::items[id.first].id];
        bazaar_sum_profit += bazaar_profit_per_hour[minion_calculator::items[id.first].id];
        drops_per_hour_sum += id.second;

        if (print_result)
        {
            std::cout << items[id.first].name << ":\n";
            std::cout << id.second << " per hour" << '\n';

            std::cout << std::fixed << profits_per_hour[minion_calculator::items[id.first].id] << " coins per hour" <<
                '\n';
            std::cout << std::fixed << bazaar_profit_per_hour[minion_calculator::items[id.first].id] <<
                " coins per hour (bazaar)" << '\n' << '\n';
        }
    }
    profit_out = sum_profit;
    bazaar_profit_out = bazaar_sum_profit;
    if (!print_result) return;
    std::cout << "Minion will fill up in " << minion.storage / drops_per_hour_sum << " hours (storage capacity: " << minion.storage << ')' << '\n';
    if (fuel.fuel_time > 0)
    {
        int needed_fuel = 86400.0f / static_cast<float>(fuel.fuel_time);
        std::cout << "Fuel will last " << fuel.fuel_time / 3600.0f << " hours" << '\n';
        std::cout << "You will need " << needed_fuel << " fuel per day" << '\n';
        std::cout << "This fuel will cost approximately " << needed_fuel * items[fuel.id].bazaar_sell_price <<
            " coins per day if bought at the bazaar" << '\n';
    }
    else
    {
        std::cout << "Fuel will last forever" << '\n';
    }

    std::cout << "Total profit per hour: " << sum_profit << '\n';
    std::cout << "Total profit per day: " << sum_profit * 24 << '\n' << '\n';

    std::cout << std::fixed << "Total profit per hour (bazaar): " << bazaar_sum_profit << '\n';
    std::cout << std::fixed << "Total profit per day (bazaar): " << bazaar_sum_profit * 24 << '\n' << '\n';

    std::cout << "Minion crafting recipe: " << '\n';

    crafting_recipe minion_recipe = recipes[minion.id];
    item_buy_requirements requirement = buy_requirements[minion.id];


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
        float items_bazaar_price = items[minion_recipe.item_id].bazaar_sell_price * minion_recipe.item_amount;
        std::cout << "You can also buy the resources at the bazaar for " << items_bazaar_price << " coins" << '\n';
    }
    else
    {
        if (!minion_recipe.craft_id.empty())
        {
            minion_item_recipe = recipes[minion_recipe.item_id];
        }
        else
        {
            minion_item_recipe = recipes[buy_requirements[minion.id].item_id];
        }

        if (drops_per_hour.find(minion_item_recipe.item_id) == drops_per_hour.end())
        {
            enchanted_item_recipe_2 = minion_item_recipe;
            enchanted_item_recipe = recipes[enchanted_item_recipe_2.item_id];
            int enchanted_item_needed_amount = enchanted_item_recipe_2.item_amount * (base_item_drops > 0 ? minion_recipe.item_amount : requirement.item_amount);
            base_item_needed_amount = enchanted_item_needed_amount * enchanted_item_recipe.item_amount;
            std::cout << enchanted_item_needed_amount << "x " << enchanted_item_recipe_2.item_id << '\n';
        }
        else
        {
            enchanted_item_recipe = minion_item_recipe;
            base_item_needed_amount = enchanted_item_recipe.item_amount * (base_item_drops > 0 ? minion_recipe.item_amount : requirement.item_amount);
        }

        std::cout << base_item_needed_amount << "x " << enchanted_item_recipe.item_id << '\n';
        float return_ratio = static_cast<float>(base_item_needed_amount) / drops_per_hour[enchanted_item_recipe.item_id];
        std::cout << "The minion returns the resource investment for its tier in " << return_ratio << " hours (or " << return_ratio / 24 << " days)" << '\n';
        float items_bazaar_price = items[enchanted_item_recipe.item_id].bazaar_sell_price * base_item_needed_amount;
        std::cout << "You can also buy the resources at the bazaar for " << items_bazaar_price << " coins" <<  '\n';
    }


    if (minion.tier - 1 > 0)
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
    auto comp = [](const minion_profit& a, const minion_profit& b) { return a.second > b.second; };
    std::priority_queue<minion_profit, std::vector<minion_profit>, decltype(comp)> top_minions(comp);
    std::priority_queue<minion_profit, std::vector<minion_profit>, decltype(comp)> top_minions_bazaar(comp);

    minion_fuel fuel;
    std::string fuel_id;
    float production_rate_boost = 0;
    bool diamond_spreading = false;

    std::cout << "Enter the fuel id:\n";
    std::cin >> fuel_id;
    fuel = minion_calculator::minion_fuels[fuel_id];

    std::cout << "Do your minions have storage? Enter the amount of storage slots or 0 if it doesn't have any\n";
    int storage_boost;
    std::cin >> storage_boost;

    std::cout << "Do your minions have diamond spreading? (y/n)\n";
    char choice;
    std::cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        diamond_spreading = true;
    }


    std::cout << "Enter the sum of production rate boost added by other means (in %)" << '\n';
    int boost;
    std::cin >> boost;
    production_rate_boost += static_cast<float>(boost);


    for (auto minion_pair : minions)
    {
        minion_pair.second.storage += storage_boost * 64;
        minion& out = minion_pair.second;
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
