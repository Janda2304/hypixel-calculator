#include <glfw3.h>

#include "drop_chance_calculator.h"
#include "farming.h"
#include "helper.hpp"
#include "imgui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "mining.h"
#include <imgui_util.h>
#include "minions/minion_calculator.h"
#include "src/color.hpp"
#include "src/minion_calculation_data.h"
#include "src/settings_data.hpp"
#include "src/style_config.h"

bool show_mgs = false;
bool show_farming = false;
bool show_minion_profit = false;
bool show_best_minion = false;
bool show_settings = false;
bool show_drop_chance = false;
bool compact_mode = false;
bool minion_simple_calculation = false;
bool ironman;
minion_calculation_data minion_calculation_data;

ImVec2 default_button_size = ImVec2(400, 50);

settings_data settings_data;

float mult_y = 0;

std::string exe_path = helper::get_exe_path();


/**
 * how many minions should be displayed in the best minion menu
 */
int best_minion_display_amount = 3;


//colors

color gray7 = {0.071f, 0.071f, 0.071f, 1};
color gray20 = {0.2f, 0.2f, 0.2f, 0.5f};
ImVec4 clicked_color = color::black(0.75f);

//


void imgui_init(GLFWwindow *window)
{
    settings_data = settings_data::load("../data/settings.json");
    best_minion_display_amount = settings_data.best_minion_display_amount;
    compact_mode = settings_data.compact_ui;
    ironman = settings_data.ironman_mode;
    minion_calculator::use_bazaar_enchanted_variants = settings_data.use_bazaar_enchanted_variants;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();


    (void) io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    io.Fonts->AddFontFromFileTTF("../fonts/Roboto-Regular.ttf", 18.0f);

    ImGui::StyleColorsDark();
}

void imgui_shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ImGui::PopFont();
}


// Main rendering loop
void render_loop(GLFWwindow *window)
{
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();



        ImGuiStyle &style = ImGui::GetStyle();

        style_config::load_config("../data/styles.json", style);
        style_config::style_vars[ImGuiStyleVar_GrabRounding] = 3;
        style.Colors[ImGuiCol_FrameBgHovered] = color::black(0.5f);
        style.Colors[ImGuiCol_ButtonHovered] = color(0.176f, 0.176f, 0.164f, 1);
        ImGui::PushStyleColor(ImGuiCol_Button, color::dark_cyan());
        ImGui::PushStyleColor(ImGuiCol_WindowBg, color::jet(1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color::dark_cyan(0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, color::moonstone(0.5f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, color::moonstone(0.8f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, color::moonstone(1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, color::kelp());
        //style_config::save_config("../data/styles.json", style);
        ImGui::NewFrame();

        // Get the size of the framebuffer
        int fb_width, fb_height;
        glfwGetFramebufferSize(window, &fb_width, &fb_height);

        int display_w, display_h;
        glfwGetWindowSize(window, &display_w, &display_h);



        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(display_w, display_h));

        ImGui::Begin("##Hypixel Calculator", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

        if (!show_mgs && !show_farming && !show_minion_profit && !show_settings && !show_best_minion && !show_drop_chance)
        {
            ImVec2 screenSize = ImGui::GetIO().DisplaySize;

            
            ImVec2 buttonPos((screenSize.x - default_button_size.x) * 0.5f, (screenSize.y - default_button_size.y) * 0.25f);
            ImGui::SetCursorPos(buttonPos);



            ImGui::PushStyleColor(ImGuiCol_FrameBg, color::transparent());
            ImGui::BeginListBox("##Main Menu", ImVec2(405, 50 * 7 + (5 * 7)));
            {
                ImGui::PopStyleColor();
            
                if (ImGui::Button("Calculate MGS (Mining Gear Score)", default_button_size))
                {
                    show_mgs = true;
                }

                if (ImGui::Button("Calculate Farming Profits", default_button_size))
                {
                    show_farming = true;
                }

                if (ImGui::Button("Calculate Minion Profit", default_button_size))
                {
                    show_minion_profit = true;
                }

                if (ImGui::Button("Calculate Best Minion", default_button_size))
                {
                    show_best_minion = true;
                }

                if (ImGui::Button("Calculate Drop Chances", default_button_size))
                {
                    show_drop_chance = true;
                }

                if (ImGui::Button("Settings", default_button_size))
                {
                    show_settings = true;
                }

                if (ImGui::Button("Exit", default_button_size))
                {
                    settings_data.save("../data/settings.json");
                    exit(0);
                }
            }
            

            ImGui::EndListBox();


            //reset styles
            ImGui::SetCursorPos(ImVec2(0, 0));
        }

        mining::show_mgs_menu(show_mgs);
        farming::show_farming_menu(show_farming);
        drop_chance_calculator::show_drop_chance_menu(show_drop_chance);

        if (show_settings)
        {
            ImGui::BeginListBox("##SETTINGS", ImVec2(500, 600));

            imgui_util::change_frame_background_color(color::black());
            imgui_util::change_item_spacing(5, 5);

            ImGui::Checkbox("Compact UI", &compact_mode);
            ImGui::Checkbox("Ironman", &ironman);
            ImGui::Checkbox("Use enchanted variants in profit calculations (bazaar only)", &minion_calculator::use_bazaar_enchanted_variants);
            ImGui::SliderInt("Best Minion Display", &best_minion_display_amount, 0, 12);

            ImGui::EndListBox();
            ImGui::PopStyleColor();

            if (imgui_util::back_button("Back to main menu (saves settings)", ImVec2(500, 50), 5))
            {
                settings_data.best_minion_display_amount = best_minion_display_amount;
                settings_data.compact_ui = compact_mode;
                settings_data.ironman_mode = ironman;
                settings_data.use_bazaar_enchanted_variants = minion_calculator::use_bazaar_enchanted_variants;
                settings_data.save("../data/settings.json");
                
                show_settings = false;
            }
            ImGui::PopStyleVar();
        }

        if (show_best_minion)
        {
            std::vector<const char *> fuel_names;

            std::vector<std::string> fuel_ids;


            for (const auto &fuel: minion_fuel::minion_fuels)
            {
                fuel_names.push_back(fuel.second.name.c_str());
                fuel_ids.push_back(fuel.first);
            }

            static char fuel_filter_buffer[256] = "";


            ImGui::BeginListBox("##Fuel Selection", ImVec2(305, 230));

            imgui_util::change_item_width(300);
            imgui_util::change_item_spacing_y(5);

            ImGui::PushStyleColor(ImGuiCol_FrameBg, color::feldgrau());

            ImGui::InputText("##Search Fuels", fuel_filter_buffer, IM_ARRAYSIZE(fuel_filter_buffer));
            ImGui::PopStyleColor();
            ImGui::BeginListBox("##Fuels");

            ImGui::PushStyleColor(ImGuiCol_Header, color::feldgrau());
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, color::black_olive(0.6f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, color::black_olive());
            for (size_t i = 0; i < fuel_names.size(); ++i)
            {
                // Convert the fuel name to lowercase
                std::string fuel_name_lowercase = fuel_names[i];
                std::transform(fuel_name_lowercase.begin(), fuel_name_lowercase.end(), fuel_name_lowercase.begin(), tolower);

                std::string fuel_filter_lowercase = fuel_filter_buffer;
                std::transform(fuel_filter_lowercase.begin(), fuel_filter_lowercase.end(), fuel_filter_lowercase.begin(), tolower);

                // Apply case-insensitive filter
                if (fuel_filter_lowercase.empty() || fuel_name_lowercase.find(fuel_filter_lowercase) != std::string::npos)
                {
                    bool is_selected = (minion_calculator::selected_fuel_index == static_cast<int>(i));

                    if (ImGui::Selectable(fuel_names[i], is_selected)) minion_calculator::selected_fuel_index = static_cast<int>(i);

                    if (is_selected) ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::PopStyleColor(3);


            ImGui::EndListBox();


            ImGui::Text("Current Fuel: %s", fuel_names[minion_calculator::selected_fuel_index]);
            ImGui::EndListBox();


            ImGui::PopStyleVar();
            imgui_util::change_item_spacing(5, 5);

            if (compact_mode)
            {
                imgui_util::change_frame_background_color(gray20);
                ImGui::SameLine();
                //TODO: check ListBox with if statement
                ImGui::BeginListBox("##CHECKMARK_GROUP", ImVec2(325, 140));
                ImGui::PopStyleColor();
            }

            ImGui::Checkbox("Diamond Spreading", &minion_calculator::diamond_spreading);


            if (compact_mode)
            {
                imgui_util::dummy(0, 15);
            }
            imgui_util::change_item_width(110);
            ImGui::SliderInt("##Storage Capacity", &minion_calculator::storage_capacity, 0, 27);


            ImGui::SameLine();

            ImGui::Text("Storage Capacity: %d items", minion_calculator::storage_capacity * 64);


            ImGui::InputInt("Other Percentage Boosts", &minion_calculator::other_boosts_percentage, 0, 0, ImGuiInputTextFlags_CharsDecimal);

            if (compact_mode)
            {
                ImGui::EndListBox();
            }

            if (imgui_util::back_button("Back to main menu", ImVec2(400, 40), 5))
            {
                show_best_minion = false;
            }


            imgui_util::dummy(0, 5);
            ImVec2 window_size = ImGui::GetWindowSize();
            float scaling_factor_x = window_size.x / 1280;
            float scaling_factor_y = window_size.y / 720;
            
            ImVec2 minion_info_size = ImVec2(1225 * scaling_factor_x, 300 * scaling_factor_y);
            /*if (best_minion_display_amount > 4)
            {
                minion_info_size.x = 1240 * scaling_factor_x;
            }*/
            if (compact_mode)
            {
                minion_info_size.y = 400 * scaling_factor_y;
            }
            std::vector<std::pair<std::string, double>> minion_profits;

            std::map<std::string, struct minion_calculation_data> calculation_datas;
            minion_calculator::selected_fuel_id = fuel_ids[minion_calculator::selected_fuel_index];
            for (const auto& minion: minion::minions)
            {
                struct minion_calculation_data data;

                minion_calculator::calc_minion_profit(minion.second, data, minion_fuel::minion_fuels[minion_calculator::selected_fuel_id], minion_calculator::diamond_spreading);
                calculation_datas.insert({minion.first, data});
            }

            // Iterate over all minions to collect profit data
            for (const auto& minion : minion::minions)
            {
                double profit = calculation_datas[minion.first].sum_profit_bazaar;
                
                if (ironman) profit = calculation_datas[minion.first].sum_profit_npc;
                
                minion_profits.emplace_back(minion.first, profit);
            }

            // Sort the vector by profit (descending order)
            std::sort(minion_profits.begin(), minion_profits.end(), [](const auto& a, const auto& b)
            {
                return a.second > b.second;
            });

        
            
            int num_minions_to_display = std::min(static_cast<int>(minion_profits.size()), best_minion_display_amount);

            if(ImGui::BeginListBox("##BEST_MINIONS", minion_info_size))
            {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, color::feldgrau());
                for (int i = 0; i < num_minions_to_display; ++i)
                {
                std::string minion_name = "##" + minion::minions[minion_profits[i].first].name;
                if(ImGui::BeginListBox(minion_name.c_str(), ImVec2(300, 150)))
                {
                    std::string profit_npc_formatted = "Profit per hour (NPC): " + helper::format_numbers(calculation_datas[minion_profits[i].first].sum_profit_npc);
                    std::string profit_bazaar_formatted = "Profit per hour (Bazaar): " + helper::format_numbers(calculation_datas[minion_profits[i].first].sum_profit_bazaar);
                    std::string profit_per_day_npc_formatted = "Profit per day (NPC): " + helper::format_numbers(calculation_datas[minion_profits[i].first].sum_profit_npc * 24);
                    std::string profit_per_day_bazaar_formatted = "Profit per day (Bazaar): " + helper::format_numbers(calculation_datas[minion_profits[i].first].sum_profit_bazaar * 24);
                    ImGui::Text("%d) Minion: %s", i + 1, minion::minions[minion_profits[i].first].name.c_str());
                    ImGui::Text(profit_npc_formatted.c_str());
                    ImGui::Text(profit_bazaar_formatted.c_str());
                
                    imgui_util::dummy(0, 5);
                
                    ImGui::Text(profit_per_day_npc_formatted.c_str());
                    ImGui::Text(profit_per_day_bazaar_formatted.c_str());
                }
                
                ImGui::EndListBox();
                if (minion_info_size.x > 1520 && minion_info_size.x < 1820)
                {
                    if (i % 5 != 4)
                    {
                        ImGui::SameLine();
                    }
                }
                else if (minion_info_size.x > 1820)
                {
                    if (i % 6 != 5)
                    {
                        ImGui::SameLine();
                    }
                }
                else if (minion_info_size.x < 1520 && minion_info_size.x > 930)
                {
                    if (i % 4 != 3)
                    {
                        ImGui::SameLine();
                    }
                }
                else if (minion_info_size.x < 930 && minion_info_size.x > 640)
                {
                    if (i % 3 != 2)
                    {
                        ImGui::SameLine();
                    }
                }
                else if (minion_info_size.x < 640)
                {
                    if (i % 2 != 1)
                    {
                        ImGui::SameLine();
                    }
                }

                if (minion_info_size.y < 300)
                {
                    if (minion_info_size.x > 1520)
                    {
                        num_minions_to_display = 5;
                    }
                    else if (minion_info_size.x > 1250 && minion_info_size.x < 1520)
                    {
                        num_minions_to_display = 4;
                    }

                }


               
               
            }
            }
            
            ImGui::EndListBox();
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        }

        if (show_minion_profit)
        {
            std::vector<const char *> minion_names;
            std::vector<const char *> fuel_names;

            std::vector<std::string> minion_ids;
            std::vector<std::string> fuel_ids;

            // Populate the vector of minion names
            for (const auto &minion: minion::minions)
            {
                minion_names.push_back(minion.second.name.c_str());
                minion_ids.push_back(minion.first);
            }


            for (const auto &fuel: minion_fuel::minion_fuels)
            {
                fuel_names.push_back(fuel.second.name.c_str());
                fuel_ids.push_back(fuel.first);
            }

            static char minion_filter_buffer[256] = "";
            static char fuel_filter_buffer[256] = "";


            if(ImGui::BeginListBox("##Minion Selection", ImVec2(305, 230)))
            {
                imgui_util::change_item_width(300);
                imgui_util::change_item_spacing_y(5);


                ImGui::InputText("##Search Minions", minion_filter_buffer, IM_ARRAYSIZE(minion_filter_buffer));
                if ( ImGui::BeginListBox("##Minions"))
                {
                    for (size_t i = 0; i < minion_names.size(); ++i)
                    {
                        if (minion_filter_buffer[0] == '\0' || strstr(minion_names[i], minion_filter_buffer) != nullptr)
                            // Apply filter
                        {
                            bool is_selected = (minion_calculator::selected_minion_index == static_cast<int>(i));

                            if (ImGui::Selectable(minion_names[i], is_selected))
                                minion_calculator::selected_minion_index = static_cast<int>(i);

                            if (is_selected) ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndListBox();
                }
            
               

                ImGui::Text("Current Minion: %s", minion_names[minion_calculator::selected_minion_index]);
                ImGui::EndListBox();
                ImGui::PopStyleVar();
            }
          

            imgui_util::change_item_spacing(10, 5);
            ImGui::SameLine();


            if(ImGui::BeginListBox("##Fuel Selection", ImVec2(305, 230)))
            {
                imgui_util::change_item_width(300);


                ImGui::InputText("Search Fuels", fuel_filter_buffer, IM_ARRAYSIZE(fuel_filter_buffer));
                if (ImGui::BeginListBox("##Fuels"))
                {
                    for (size_t i = 0; i < fuel_names.size(); ++i)
                    {
                        // Convert the fuel name to lowercase
                        std::string fuel_name_lowercase = fuel_names[i];
                        std::transform(fuel_name_lowercase.begin(), fuel_name_lowercase.end(), fuel_name_lowercase.begin(),
                                       tolower);

                        std::string fuel_filter_lowercase = fuel_filter_buffer;
                        std::transform(fuel_filter_lowercase.begin(), fuel_filter_lowercase.end(),
                                       fuel_filter_lowercase.begin(), tolower);

                        // Apply case insensitive filter
                        if (fuel_filter_lowercase.empty() || fuel_name_lowercase.find(fuel_filter_lowercase) !=
                            std::string::npos)
                        {
                            bool is_selected = (minion_calculator::selected_fuel_index == static_cast<int>(i));

                            if (ImGui::Selectable(fuel_names[i], is_selected))
                                minion_calculator::selected_fuel_index = static_cast<int>(i);

                            if (is_selected) ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndListBox();
                }
                ImGui::Text("Current Fuel: %s", fuel_names[minion_calculator::selected_fuel_index]);
                ImGui::EndListBox();
            }
        


            imgui_util::dummy(0, 10);

            ImGui::Checkbox("Diamond Spreading", &minion_calculator::diamond_spreading);
            if (compact_mode)
            {
                ImGui::SameLine();
                imgui_util::dummy(10, 0);
                ImGui::SameLine();
            }
            ImGui::Checkbox("Simple Calculation", &minion_simple_calculation);

            imgui_util::change_item_width(155);
            ImGui::SliderInt("##Storage Capacity", &minion_calculator::storage_capacity, 0, 27);


            ImGui::SameLine();

            ImGui::Text("Storage Capacity: %d items", minion_calculator::storage_capacity * 64);

            if (compact_mode)
            {
                ImGui::SameLine();
                imgui_util::dummy(10, 0);
                ImGui::SameLine();
            }

            ImGui::InputInt("Other Percentage Boosts", &minion_calculator::other_boosts_percentage, 0, 0,
                            ImGuiInputTextFlags_CharsDecimal);


            if (ImGui::Button("Calculate", ImVec2(400, 40)))
            {
                minion_calculator::selected_fuel_id = fuel_ids[minion_calculator::selected_fuel_index];
                minion_calculator::selected_minion_id = minion_ids[minion_calculator::selected_minion_index];

                minion_calculator::calc_minion_profit(minion::minions[minion_calculator::selected_minion_id],
                                                      minion_calculation_data,
                                                      minion_fuel::minion_fuels[minion_calculator::selected_fuel_id],
                                                      minion_calculator::diamond_spreading);
            }


            imgui_util::dummy(0, 5);
            if(ImGui::BeginListBox("##Minion Info", ImVec2(400, 200)))
            {
                ImGui::Text("Minion Production Rate: %f s", minion_calculation_data.minion_production_rate);
                ImGui::Dummy(ImVec2(0, 2));
                ImGui::Text("Minion will fill up in %f hours", minion_calculation_data.storage_fill_up_time);
                if (!minion_simple_calculation)
                {
                    ImGui::Text("DROPS:");
                    for (const auto &drop: minion_calculation_data.minion_drops)
                    {
                    if (compact_mode) ImGui::BeginListBox("##DROPINFO", ImVec2(270, 95));

                    ImGui::Text("%s : ", item::items[drop.first].name.c_str());
                    ImGui::Text("Production per hour: %f", minion_calculation_data.drops_per_hour[drop.first]);
                    ImGui::Text("Profit per hour (NPC): %f", minion_calculation_data.profits_per_hour[drop.first]);
                    ImGui::Text("Profit per hour (Bazaar): %f",
                                minion_calculation_data.bazaar_profit_per_hour[drop.first]);

                    if (compact_mode) ImGui::EndListBox();

                    else imgui_util::dummy(0, 5);
                    }

                    for (auto skill: minion_calculation_data.skill_xp_drops)
                    {
                    ImGui::Text("%s xp (per hour): %f", to_string(skill.first), skill.second);
                    }

                    imgui_util::dummy(0, 10);
                }
                else
                {
                    ImGui::Text("Sum Profit per hour (NPC): %f", minion_calculation_data.sum_profit_npc);
                    ImGui::Text("Sum Profit per hour (Bazaar): %f", minion_calculation_data.sum_profit_bazaar);
                    imgui_util::dummy(0, 10);

                    ImGui::Text("Sum profit per day (NPC): %f", minion_calculation_data.sum_profit_npc * 24);
                    ImGui::Text("Sum profit per day (Bazaar): %f", minion_calculation_data.sum_profit_bazaar * 24);
                }

                if (minion_calculation_data.minion_recipe != crafting_recipe())
                {
                    ImGui::Text("Minion Recipe: %s", minion_calculation_data.minion_recipe.get_recipe_string().c_str());
                    ImGui::Text("Return Ratio: %f hours", minion_calculation_data.minion_return_ratio);
                }
                ImGui::EndListBox();
            }


            

            ImGui::PopStyleVar();
            if (ImGui::Button("Back to main menu", ImVec2(400, 40)))
            {
                show_minion_profit = false;
            }
        }
        //minion_calculator::show_minion_profit_menu(show_minion_profit);

        ImGui::End();


        // Rendering
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}

int main()
{
    // initialize the data
    minion_calculator::init_fuels("../data/minion_fuels.json");
    minion_calculator::init_minions("../data/minions.json");
    minion_calculator::init_items("../data/items.json");
    minion_calculator::init_recipes("../data/item_recipes.json");
    //minion_calculator::init_recipes("../data/minion_recipes.json");
    minion_calculator::init_recipes("../data/item_recipes_new.json");
    minion_calculator::init_buy_requirements("../data/item_buy_requirements.json");


    // Initialize GLFW
    if (!glfwInit()) return -1;


    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Multipurpose Hypixel Skyblock Calculator", nullptr, nullptr);

    if (!window) return -1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    imgui_init(window);

    render_loop(window);

    imgui_shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
