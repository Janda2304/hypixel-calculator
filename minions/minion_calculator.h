#pragma once
#include <string>
#include <map>

#include "../src/minion_fuel.h"
#include "../src/minion.h"


class minion_calculator
{
public:
  

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

    static void calc_minion_profit(minion &minion, float &profit_out, float &bazaar_profit_out, minion_fuel &fuel,
                                   bool diamond_spreading = false, bool manual_calculation = true,
                                   bool print_result = true);

    static void calc_best_minion();
};
