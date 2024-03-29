﻿#include "farming.h"

#include <imgui.h>

#include "imgui/imgui_util.h"
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
 * \param final_profit final profit out
 * \param final_profit_bazaar final bazaar profit out
 * \param final_drop final drop out
 * \param type_out crop type out 
 */
void farming::calculate_crop_profit(crop_type type, float& final_profit, float& final_profit_bazaar, int& final_drop, crop_type& type_out)
{
    crop crop = crop_map()[type];

    
    float sell_price = crop.item.sell_price;
    float bazaar_sell_price = crop.item.bazaar_sell_price;
    
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
        
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 5));

        ImGui::InputInt("##Crop Break Speed", &farming::crop_break_speed, 0, 0, ImGuiInputTextFlags_CharsDecimal);
        ImGui::SameLine();
        ImGui::Text("Crop Break Speed: %d/s (max 20)", farming::crop_break_speed);

        ImGui::Dummy(ImVec2(0,5));

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
            
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
        ImGui::Dummy(ImVec2(0, 5));
            
        ImGui::Text("Current Crop: %s", to_string(type_display));
            
        ImGui::Text("Profit Per Hour: %f", final_profit);
        ImGui::Text("Profit Per Hour (Bazaar): %f", final_profit_bazaar);
        ImGui::Text("Crops Farmed Per Hour: %f", final_drop);
        
        imgui_util::change_button_color(0.8f, 0.2f, 0.2f, 0.6f);
        imgui_util::change_button_hover_color(0.8f, 0.2f, 0.2f, 1.0f);
        
        if (ImGui::Button("Back to main menu", ImVec2(250, 50)))
        {
            show_farming = false;
        }
        ImGui::PopStyleColor();
    }
}



