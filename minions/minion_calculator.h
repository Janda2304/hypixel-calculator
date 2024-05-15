#pragma once
#include <string>
#include <map>

#include "../src/minion_fuel.h"
#include "../src/minion.h"
#include "../src/minion_calculation_data.h"


class minion_calculator
{
public:

    static bool diamond_spreading;
    /**
     * if the calculator should use enchanted variants of the items for the bazaar price calculation
     */
    static bool use_bazaar_enchanted_variants;
    static int storage_capacity;
    
    static std::string selected_minion_id;
    static int selected_minion_index;

    static std::string selected_fuel_id;
    static int selected_fuel_index;

    static int other_boosts_percentage;
 

    /**
     * \brief used to replace weird ids with the correct ones (e.g. "SLIME_BALL" to "SLIMEBALL") <br> the ids are replaced because no one can tell that e.g. LOG_2:1 is a birch log or something
     */
    static std::map<std::string, std::string> odd_items_ids;

    static void init_fuels(const std::string &path);

    static void init_items(const std::string &path);

    static void init_minions(const std::string &path);

    static void init_recipes(const std::string &path);

    static void init_buy_requirements(const std::string &path);

    static void save_fuels(const std::string &path);

    static void save_items(const std::string &path);

    static void save_minions(const std::string &path);

    static void calc_minion_profit(::minion minion, minion_calculation_data& calculation_data, const minion_fuel& fuel, bool diamond_spreading);

    static void show_minion_profit_menu(bool &show_minion_profit);

    static void calc_best_minion();
};
