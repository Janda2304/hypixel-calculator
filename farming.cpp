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
        {mushroom, {mushroom, item::items["MUSHROOM"], 1}},
        {cactus, {cactus, item::items["CACTUS"], 2}}
    };
    return _crop_map;
}


int farming::crop_break_speed = 17;
int farming::farming_fortune = 0;

/**
 * \brief 
 * \param type crop type
 */
void farming::calculate_crop_profit(crop_type type)
{
    crop crop = crop_map()[type];

    
    float sell_price = crop.item.sell_price;
    float bazaar_sell_price = crop.item.bazaar_sell_price;

    
    float drop_amount = crop.base_drop * (static_cast<float>(farming_fortune) * 0.01f);

    
    float profit_per_one = drop_amount * sell_price;
    float profit_per_one_bazaar = drop_amount * bazaar_sell_price;

    float final_profit_bazaar = profit_per_one_bazaar * static_cast<float>(crop_break_speed) * 3600;
    float final_profit = profit_per_one * static_cast<float>(crop_break_speed) * 3600;
    
    int final_drop = drop_amount * crop_break_speed * 3600; 

    
    helper::clear();
    
    std::cout << crop.item.name  << '\n' << '\n';
    
    std::cout << "Sell Price: " << sell_price << '\n';
    std::cout << "Bazaar Sell Price: " << bazaar_sell_price << '\n';

    std::cout << "You'll make approximately " << profit_per_one << " (if sold to NPC) or " << profit_per_one_bazaar << " (if sold to bazaar) per one crop" << '\n';
    std::cout << "Final Profit (Per 1 hour): " << static_cast<int>(final_profit) << " (if sold to NPC) or " << static_cast<int>(final_profit_bazaar) << " (if sold to bazaar)" << '\n';
    std::cout << "Final Crops Farmed (Per 1 hour):" << final_drop << '\n';
    std::cout << "*Calculated with 85% efficiency" << '\n' << '\n';
    
    std::cout << "Base Drop: " << crop.base_drop << '\n';
    std::cout << "Farming Fortune: " << farming_fortune << '\n';
    
    helper::pause();
}

void farming::calculate_farming_profit()
{
    helper::clear();
    std::cout << "enter your farming fortune:\n";
    std::cin >> farming_fortune;
    std::cout << "enter your desired crop type:\n1)wheat\n2)carrot\n3)potato\n4)nether wart\n5)sugar cane\n6)melon\n7)pumpkin\n8)cocoa bean\n9) mushroom\n10)cactus\nyour crop:";

    int int_type;
    std::cin >> int_type;
    calculate_crop_profit(static_cast<crop_type>(int_type));
}
