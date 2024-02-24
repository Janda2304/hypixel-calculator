#pragma once
#include <map>

#include "global_src/item.h"


enum crop_type
{
    wheat = 1,
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

struct crop
{
    crop_type type;
    item item;
    float base_drop;
};


class farming
{
   
public:
    static void calculate_crop_profit(crop_type type);

    static void calculate_farming_profit();
    
    static std::map<crop_type, crop>& crop_map();
    

    static int crop_break_speed;

    static int farming_fortune;
    
};






