﻿#include "mining.h"
#include <imgui.h>


int mining::mining_speed;
int mining::mining_fortune;
float mining::pristine;




void mining::calculate_mgs(int mining_speed, int mining_fortune, float pristine, float& final_profit, float& mgs)
{
    mgs = mining_speed / 100 * (1 + mining_fortune / 100) * (1 + pristine * 0.79);
    final_profit = mgs * 1080;
}

void mining::show_mgs_menu(bool &show_mgs)
{
    if (show_mgs)
    {
        float final_profit;
        float mgs;
        ImGui::PushItemWidth(200);
        ImGui::InputInt("Mining Speed", &mining::mining_speed, 0, 0, ImGuiInputTextFlags_CharsDecimal);
        ImGui::Text("Mining Speed: %d", mining::mining_speed);
        ImGui::Dummy(ImVec2(0, 5));
            
        ImGui::InputInt("Mining Fortune", &mining::mining_fortune, 0, 0, ImGuiInputTextFlags_CharsDecimal);
        ImGui::Text("Mining Fortune: %d", mining::mining_fortune);
        ImGui::Dummy(ImVec2(0, 5));
            
        ImGui::InputFloat("Pristine", &mining::pristine, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal);
        ImGui::Text("Pristine: %f", mining::pristine);
            
        ImGui::Dummy(ImVec2(0, 10)); //add vertical spacing

        mining::calculate_mgs(mining::mining_speed, mining::mining_fortune, mining::pristine, final_profit, mgs);

        ImGui::Text("Your Mining Gear Score (MGS) is: %f", mgs);
        ImGui::Text("You will make approximately %f coins a hour", final_profit);

        if (ImGui::Button("Back to main menu", ImVec2(400, 50)))
        {
            show_mgs = false;
        }
    }
}

