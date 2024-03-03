#include "farming.h"
#include <iostream>

#include "helper.hpp"
#include "minions/minion_calculator.h"


std::map<crop_type, crop>& farming::crop_map()
{
    static std::map<crop_type, crop> _crop_map =
    {
        {wheat, {wheat, item::items["WHEAT"], 1}},
        {carrot, {carrot, item::items["CARROT"], 3.7f}},
        {potato, {potato, item::items["POTATO"], 3.7f}},
        {nether_wart, {nether_wart, item::items["NETHER_WART"], 1}},
        {sugar_cane, {sugar_cane, item::items["SUGAR_CANE"], 2}},
        {melon, {melon, item::items["MELON"], 5}},
        {pumpkin, {pumpkin, item::items["PUMPKIN"], 1}},
        {cocoa_bean, {cocoa_bean, item::items["COCOA_BEANS"], 3}},
        {mushroom, {mushroom, item::items["RED_MUSHROOM"], 1}},
        {cactus, {cactus, item::items["CACTUS"], 2}}
    };
    return _crop_map;
}


int farming::crop_break_speed = 17;
int farming::farming_fortune = 0;

/**
 * \brief 
 * \param type crop type
 * \param final_profit
 * \param final_profit_bazaar
 * \param final_drop
 */
void farming::calculate_crop_profit(crop_type type, float& final_profit, float& final_profit_bazaar, int& final_drop, crop_type& type_out)
{
    crop crop = crop_map()[type];

    
    float sell_price = crop.item.sell_price;
    float bazaar_sell_price = crop.item.bazaar_sell_price;

    
    float drop_amount = crop.base_drop * (static_cast<float>(farming_fortune) * 0.01f);

    
    float profit_per_one = drop_amount * sell_price;
    float profit_per_one_bazaar = drop_amount * bazaar_sell_price;

    final_profit_bazaar = profit_per_one_bazaar * static_cast<float>(crop_break_speed) * 3600;
    final_profit = profit_per_one * static_cast<float>(crop_break_speed) * 3600;
    
    final_drop = drop_amount * crop_break_speed * 3600;

    type_out = crop.type;
}
