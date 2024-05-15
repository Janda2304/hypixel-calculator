#include "farming.h"

#include <imgui.h>

#include "helper.hpp"
#include "imgui/imgui_util.h"
#include "minions/minion_calculator.h"
#include "src/color.hpp"

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
 * \param final_profit final profit out
 * \param final_profit_bazaar final bazaar profit out
 * \param final_drop final drop out
 * \param type_out crop type out 
 */
void farming::calculate_crop_profit(crop_type type, float& final_profit, float& final_profit_bazaar, int& final_drop, crop_type& type_out)
{
    crop crop = crop_map()[type];

    
    float sell_price = crop.itm.sell_price;
    float bazaar_sell_price = crop.itm.bazaar_sell_price;
    
    float drop_amount = crop.base_drop * (1 + (static_cast<float>(farming_fortune) * 0.01f));

    
    float profit_per_one = drop_amount * sell_price;
    float profit_per_one_bazaar = drop_amount * bazaar_sell_price;

    final_profit_bazaar = profit_per_one_bazaar * static_cast<float>(crop_break_speed) * 3600;
    final_profit = profit_per_one * static_cast<float>(crop_break_speed) * 3600;
    
    final_drop = drop_amount * crop_break_speed * 3600;

    type_out = crop.type;
}

float final_profit;
float final_profit_bazaar;
int final_drop;
crop_type type_display;








void farming::show_farming_menu(bool &show_farming)
{
    if (show_farming)
    {
        ImGui::PushItemWidth(200);
        ImGui::InputInt("##Farming Fortune", &farming::farming_fortune, 0, 0, ImGuiInputTextFlags_CharsDecimal);
        ImGui::SameLine();
        ImGui::Text("Farming Fortune: %d", farming::farming_fortune);

        ImGui::SliderInt("##Crop Break Speed", &farming::crop_break_speed, 1, 20);
        ImGui::SameLine();
        ImGui::Text("Crop Break Speed: %d/s", farming::crop_break_speed);

        ImGui::Dummy(ImVec2(0,5));


        imgui_util::change_item_spacing(4,4);
            
        for (int i = 0; i < 10; ++i)
        {
            crop_type type = static_cast<crop_type>(i);
            if (ImGui::Button(to_string(type), ImVec2(100, 30)))
            {
                farming::calculate_crop_profit(type, final_profit, final_profit_bazaar, final_drop, type_display);
            }
            if (i % 2 == 0)
            {
                ImGui::SameLine();
            }
        }
        imgui_util::dummy(0, 10);
            
        ImGui::Text("Current Crop: %s", to_string(type_display));

        std::string profit_str = "Profit per hour: " +  helper::format_numbers(final_profit);
        std::string profit_bazaar_str = "Profit per hour (Bazaar): " + helper::format_numbers(final_profit_bazaar);
        std::string drop_str = "Crops farmed per hour: " + helper::format_numbers(final_drop);
            
        ImGui::Text(profit_str.c_str());
        ImGui::Text(profit_bazaar_str.c_str());
        ImGui::Text(drop_str.c_str());

        
        if (imgui_util::back_button("Back to main menu", ImVec2(250, 50), 5))
        {
            show_farming = false;
        }
        ImGui::PopStyleVar();
    }
}



