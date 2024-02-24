#include "buy_requirements.h"

#include <iostream>

void item_buy_requirements::print_buy_requirements() const
{
    std::cout << "Offer ID: " << offer_id << '\n';
    std::cout << "Item ID: " << item_id << '\n';
    std::cout << "Item Amount: " << item_amount << '\n';
    std::cout << "Coin Price: " << coin_price << '\n';
    std::cout << "You will also need: ";
    for (const auto& cost : other_cost)
    {
        std::cout << cost << ' ';
    }
    std::cout << '\n';
}

void item_buy_requirements::print_buy_requirements_short() const
{
    std::cout << item_amount << "x " << item_id << '\n';
    std::cout << "Coin Price: " << coin_price << '\n';
    if (!other_cost.empty())
    {
        std::cout << "You will also need: ";
    }
    for (const auto& cost : other_cost)
    {
        std::cout << cost << '\n';
    }
}

nlohmann::json item_buy_requirements::serialize() const
{
    nlohmann::json json_data;
    json_data["offer_id"] = offer_id;
    json_data["item_id"] = item_id;
    json_data["item_amount"] = item_amount;
    json_data["coin_price"] = coin_price;
    json_data["other_cost"] = other_cost;
    return json_data;
}

item_buy_requirements item_buy_requirements::deserialize(const nlohmann::json& json_data)
{
    item_buy_requirements buy_requirements;
    buy_requirements.offer_id = json_data["offer_id"];
    buy_requirements.item_id = json_data["item_id"];
    buy_requirements.item_amount = json_data["item_amount"];
    buy_requirements.coin_price = json_data["coin_price"];
    buy_requirements.other_cost = json_data["other_cost"].get<std::vector<std::string>>();
    return buy_requirements;
}
