﻿#pragma once
#include <iostream>
#include <string>
#include <map>

#include "../src/buy_requirements.h"
#include "../src/minion_fuel.h"
#include "../src/crafting_recipe.h"
#include "../src/minion.h"


class minion_calculator
{
public:
    static std::map<std::string, minion_fuel> minion_fuels;
    static std::map<std::string, crafting_recipe> recipes;
    static std::map<std::string, item_buy_requirements> buy_requirements;
    
    /**
     * \brief used to replace weird ids with the correct ones (e.g. "SLIME_BALL" to "SLIMEBALL")
     */
    static std::map<std::string, std::string> odd_items_ids;

    static void init_fuels(const std::string& path);

    static void init_items(const std::string& path);

    static void init_minions(const std::string& path);

    static void init_recipes(const std::string& path);

    static void init_buy_requirements(const std::string& path);

    static void save_fuels(const std::string& path);
    
    static void save_items(const std::string& path);
    
    static void save_minions(const std::string& path);

    static void calc_minion_profit(minion& minion, float& profit_out, float& bazaar_profit_out, minion_fuel& fuel, bool diamond_spreading = false, bool manual_calculation = true, bool print_result = true);

    static void calc_best_minion();

    
};