#pragma once
#include <map>

#include <item.h>


enum crop_type
{
    wheat,
    carrot,
    potato,
    nether_wart,
    sugar_cane,
    melon,
    pumpkin,
    cocoa_bean,
    mushroom,
    cactus
};

const inline char *to_string(const crop_type e)
{
    switch (e)
    {
        case wheat: return "wheat";
        case carrot: return "carrot";
        case potato: return "potato";
        case nether_wart: return "nether wart";
        case sugar_cane: return "sugar cane";
        case melon: return "melon";
        case pumpkin: return "pumpkin";
        case cocoa_bean: return "cocoa bean";
        case mushroom: return "mushroom";
        case cactus: return "cactus";
        default: return "unknown";
    }
}

struct crop
{
    crop_type type;
    item itm;
    float base_drop;
};


class farming
{
   
public:
    static void calculate_crop_profit(crop_type type, float &final_profit, float &final_profit_bazaar, int &final_drop, crop_type& type_out);

    static void calculate_farming_profit(int farming_fortune, crop_type type, float &final_profit, float &final_profit_bazaar, int &final_drop);

    static void show_farming_menu(bool &show_farming);
    
    static std::map<crop_type, crop>& crop_map();
    

    static int crop_break_speed;

    static int farming_fortune;
    
};






