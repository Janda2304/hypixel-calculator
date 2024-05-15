#include "mining.h"
#include <imgui.h>

#include "helper.hpp"
#include "imgui/imgui_util.h"
#include "src/color.hpp"


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
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 8));
        
        ImGui::InputInt("##Mining Speed", &mining::mining_speed, 0, 0, ImGuiInputTextFlags_CharsDecimal);
        ImGui::SameLine();
        ImGui::Text("Mining Speed: %d", mining::mining_speed);
            
        ImGui::InputInt("##Mining Fortune", &mining::mining_fortune, 0, 0, ImGuiInputTextFlags_CharsDecimal);
        ImGui::SameLine();
        ImGui::Text("Mining Fortune: %d", mining::mining_fortune);
            
        ImGui::InputFloat("##Pristine", &mining::pristine, 0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_CharsDecimal);
        ImGui::SameLine();
        ImGui::Text("Pristine: %f", mining::pristine);


        mining::calculate_mgs(mining::mining_speed, mining::mining_fortune, mining::pristine, final_profit, mgs);

        std::string mgs_text = "Your Mining Gear Score (MGS) is: " + helper::format_numbers(mgs);
        std::string profit_text = "You will make approximately " + helper::format_numbers(final_profit) + " coins a hour";

        ImGui::Text(mgs_text.c_str());
        ImGui::Text(profit_text.c_str());


        if (imgui_util::back_button("Back to main menu", ImVec2(250, 50), 5))
        {
            show_mgs = false;
        }
        ImGui::PopStyleVar();
    }
}

