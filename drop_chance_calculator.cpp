#include "drop_chance_calculator.h"

#include <imgui.h>

#include "imgui/imgui_util.h"
#include "src/color.hpp"


float drop_chance_calculator::magic_find;
float drop_chance_calculator::default_drop_chance;
float drop_chance_calculator::drop_chance;
float drop_chance_calculator::pet_luck;
bool drop_chance_calculator::pet_calculation;


void drop_chance_calculator::show_drop_chance_menu(bool &show_drop_chance)
{
    if (show_drop_chance)
    {
        imgui_util::change_item_spacing_y(10);
        imgui_util::change_item_width(150);
        ImGui::InputFloat("Item's Default Drop Chance", &default_drop_chance);
        ImGui::InputFloat("Magic Find", &magic_find);
        ImGui::Checkbox("Pet Calculation", &pet_calculation);
        if (pet_calculation)
        {
            ImGui::InputFloat("Pet Luck", &pet_luck);
        }

        if (default_drop_chance >= 5)
        {
            imgui_util::change_text_color(color::persian_red());
            ImGui::Text("Warning!\nMagic Find does not affect drops from most Mobs with a higher drop chance than 5%");
            imgui_util::reset_color();
        }

        if (pet_calculation) drop_chance = default_drop_chance * (1 + (magic_find + pet_luck / 100));
        
        else drop_chance = default_drop_chance * (1 + (magic_find / 100));

        ImGui::Text("Item drop chance is: %f", drop_chance);
        ImGui::Text("That also means 1/%d", static_cast<int>(100 / drop_chance));
        
        
        imgui_util::change_button_color(color::persian_red(0.6f));
        imgui_util::change_button_hover_color(color::persian_red(1));
        if (ImGui::Button("Back to main menu", ImVec2(250, 50)))
        {
            show_drop_chance = false;
        }
    }
}
