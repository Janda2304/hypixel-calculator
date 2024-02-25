#pragma once
#include <string>
#include <vector>

#include "../nlohmann/json.hpp"

struct item_buy_requirements
{
    std::string offer_id;
    std::string item_id;
    int item_amount;
    int coin_price;
    std::vector<std::string> other_cost;

    void print_buy_requirements() const;

    void print_buy_requirements_short() const;

    nlohmann::json serialize() const;

    static item_buy_requirements deserialize(const nlohmann::json& json_data);

    static std::map<std::string, item_buy_requirements> buy_requirements;
};
