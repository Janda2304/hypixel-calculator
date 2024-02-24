#pragma once
#include <string>
#include <iostream>
#include "../single_include/nlohmann/json.hpp"





struct item
{
    std::string name;
    std::string id;
    float sell_price;
    float bazaar_sell_price;

    nlohmann::json serialize() const;

    static item deserialize(const nlohmann::json& json_data, nlohmann::json& bazaar_data);

    void print_item_info() const;
};
