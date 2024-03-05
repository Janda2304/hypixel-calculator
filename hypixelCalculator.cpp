#include <GLFW/glfw3.h>

#include "farming.h"
#include "imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "mining.h"
#include "imgui/imgui_util.h"
#include "minions/minion_calculator.h"
#include "src/color.hpp"
#include "src/minion_calculation_data.h"

bool show_mgs = false;
bool show_farming = false;
bool show_minion_profit = false;
bool show_best_minion = false;
bool show_settings = false;
bool compact_ui = false;
bool minion_simple_calculation = false;

minion_calculation_data minion_calculation_data;

ImVec2 default_button_size = ImVec2(400, 50);


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
        ImGui::NewFrame();

        // Get the size of the framebuffer
        int fb_width, fb_height;
        glfwGetFramebufferSize(window, &fb_width, &fb_height);

        // Get the position of the window
        int win_width, win_height;
        glfwGetWindowSize(window, &win_width, &win_height);

        // Calculate ImGui window position offset
        ImVec2 win_pos_offset = ImVec2(static_cast<float>(win_width - fb_width) / 2.0f,
                                       static_cast<float>(win_height - fb_height));

        ImGui::SetNextWindowPos(win_pos_offset);
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(fb_width), static_cast<float>(fb_height)));

        ImGui::Begin("Hypixel Calculator", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);


        imgui_util::change_text_color(color::white());
        imgui_util::change_background_color(gray7);
        imgui_util::change_button_hover_color(gray20);
        imgui_util::change_button_color(0, 0, 0, 0.5f);
        imgui_util::change_frame_background_color(0.25f, 0.25f, 0.25f, 0.5f);
        imgui_util::change_item_rounding(5);
        imgui_util::change_button_clicked_color(clicked_color);

        imgui_util::change_slider_grab_color(color::green());
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0, 0.7f, 0, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, gray20);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, clicked_color);
        ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0, 0.5f, 0, 1.0f));


        if (!show_mgs && !show_farming && !show_minion_profit && !show_settings && !show_best_minion)
        {
            ImVec2 screenSize = ImGui::GetIO().DisplaySize;

            ImVec2 buttonPos((screenSize.x - default_button_size.x) * 0.5f,
                             (screenSize.y - default_button_size.y) * 0.3f);
            ImGui::SetCursorPos(buttonPos);

            //apply styles
            imgui_util::change_item_spacing_y(10);


            ImGui::BeginListBox("##Main Menu", ImVec2(410, 360));

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

            if (ImGui::Button("Settings", default_button_size))
            {
                show_settings = true;
            }

            if (ImGui::Button("Exit", default_button_size))
            {
                exit(0);
            }

            ImGui::EndListBox();


            //reset styles
            ImGui::SetCursorPos(ImVec2(0, 0));
            imgui_util::reset_item_spacing();
        }

        mining::show_mgs_menu(show_mgs);
        farming::show_farming_menu(show_farming);

        if (show_settings)
        {
            ImGui::BeginListBox("##SETTINGS", ImVec2(500, 600));

            imgui_util::change_frame_background_color(color::black());
            imgui_util::change_item_spacing(5, 5);

            ImGui::Checkbox("UI Compact Mode", &compact_ui);
            ImGui::SliderInt("Best Minion Display", &best_minion_display_amount, 0, 10);

            ImGui::EndListBox();
            ImGui::PopStyleColor();

            imgui_util::change_button_color(color::persian_red(0.6f));
            imgui_util::change_button_hover_color(color::persian_red(1));

            if (ImGui::Button("Back to main menu", ImVec2(500, 50)))
            {
                show_settings = false;
            }
            ImGui::PopStyleColor();
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


            ImGui::InputText("Search Fuels", fuel_filter_buffer, IM_ARRAYSIZE(fuel_filter_buffer));
            ImGui::BeginListBox("##Fuels");


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

            imgui_util::change_item_spacing_y(10);
            ImGui::Text("Current Fuel: %s", fuel_names[minion_calculator::selected_fuel_index]);
            ImGui::EndListBox();


            imgui_util::change_item_spacing(5, 5);

            if (compact_ui)
            {
                imgui_util::change_frame_background_color(gray20);
                ImGui::SameLine();
                ImGui::BeginListBox("##CHECKMARK_GROUP", ImVec2(360, 140));
                ImGui::PopStyleColor();
            }

            ImGui::Checkbox("Diamond Spreading", &minion_calculator::diamond_spreading);
            ImGui::Checkbox("Simple Calculation", &minion_simple_calculation);


            if (compact_ui)
            {
                imgui_util::dummy(0, 15);
            }
            imgui_util::change_item_width(155);
            ImGui::SliderInt("##Storage Capacity", &minion_calculator::storage_capacity, 0, 27);


            ImGui::SameLine();

            ImGui::Text("Storage Capacity: %d items", minion_calculator::storage_capacity * 64);


            ImGui::InputInt("Other Percentage Boosts", &minion_calculator::other_boosts_percentage, 0, 0,
                            ImGuiInputTextFlags_CharsDecimal);

            if (compact_ui)
            {
                ImGui::EndListBox();
            }


            if (ImGui::Button("Calculate", ImVec2(400, 40)))
            {
                minion_calculator::selected_fuel_id = fuel_ids[minion_calculator::selected_fuel_index];

                minion_calculator::calc_minion_profit(minion::minions[minion_calculator::selected_minion_id],
                                                      minion_calculation_data,
                                                      minion_fuel::minion_fuels[minion_calculator::selected_fuel_id],
                                                      minion_calculator::diamond_spreading);
            }


            ImGui::SameLine();
            imgui_util::change_button_color(color::persian_red(0.6f));
            imgui_util::change_button_hover_color(color::persian_red(1));
            if (ImGui::Button("Back to main menu", ImVec2(400, 40)))
            {
                show_best_minion = false;
            }
            imgui_util::reset_color();


            imgui_util::dummy(0, 5);
            ImVec2 minion_info_size = ImVec2(1265, 250);
            if (compact_ui)
            {
                minion_info_size.y = 375;
            }
            std::vector<std::pair<std::string, double>> minion_profits;

            std::map<std::string, struct minion_calculation_data> calculation_datas;

            for (const auto& minion: minion::minions)
            {
                struct minion_calculation_data data;
                minion_calculator::calc_minion_profit(minion.second, data, minion_fuel::minion_fuels[fuel_ids[0]], minion_calculator::diamond_spreading);
                calculation_datas.insert({minion.first, data});
            }

            // Iterate over all minions to collect profit data
            for (const auto& minion : minion::minions)
                {
                double profit = calculation_datas[minion.first].sum_profit_npc;
                minion_profits.emplace_back(minion.first, profit);
            }

            // Sort the vector by profit (descending order)
            std::sort(minion_profits.begin(), minion_profits.end(), [](const auto& a, const auto& b) {
                return a.second > b.second;
            });

            // Display the top 3 minions with highest profit
            int num_minions_to_display = std::min(static_cast<int>(minion_profits.size()), 3);
            for (int i = 0; i < num_minions_to_display; ++i) {
                std::string minion_id = minion_profits[i].first;
                double profit = minion_profits[i].second;
    
                // Display minion info for the current top minion
                ImGui::BeginListBox("##Minion Info", minion_info_size);
                ImGui::Text("Minion ID: %s", minion_id.c_str());
                ImGui::Text("Profit per hour: %f", profit);
                ImGui::EndListBox();
            }


            /*if (ImGui::Button("Back to main menu", ImVec2(400, 40)))
            {
                show_best_minion = false;
            }*/
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


            ImGui::BeginListBox("##Minion Selection", ImVec2(305, 230));

            imgui_util::change_item_width(300);
            imgui_util::change_item_spacing_y(5);


            ImGui::InputText("##Search Minions", minion_filter_buffer, IM_ARRAYSIZE(minion_filter_buffer));
            ImGui::BeginListBox("##Minions");


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

            ImGui::Text("Current Minion: %s", minion_names[minion_calculator::selected_minion_index]);
            ImGui::EndListBox();

            imgui_util::change_item_spacing_x(10);
            ImGui::SameLine();


            ImGui::BeginListBox("##Fuel Selection", ImVec2(305, 230));

            imgui_util::change_item_width(300);
            imgui_util::change_item_spacing_y(5);


            ImGui::InputText("Search Fuels", fuel_filter_buffer, IM_ARRAYSIZE(fuel_filter_buffer));
            ImGui::BeginListBox("##Fuels");


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

            ImGui::Text("Current Fuel: %s", fuel_names[minion_calculator::selected_fuel_index]);
            ImGui::EndListBox();


            imgui_util::dummy(0, 10);
            imgui_util::change_item_spacing_y(5);

            ImGui::Checkbox("Diamond Spreading", &minion_calculator::diamond_spreading);
            if (compact_ui)
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

            if (compact_ui)
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
            ImGui::BeginListBox("##Minion Info", ImVec2(400, 200));


            ImGui::Text("Minion Production Rate: %f s", minion_calculation_data.minion_production_rate);
            ImGui::Dummy(ImVec2(0, 2));
            ImGui::Text("Minion will fill up in %f hours", minion_calculation_data.storage_fill_up_time);
            if (!minion_simple_calculation)
            {
                ImGui::Text("DROPS:");
                for (const auto &drop: minion_calculation_data.minion_drops)
                {
                    if (compact_ui) ImGui::BeginListBox("##DROPINFO", ImVec2(270, 95));

                    ImGui::Text("%s : ", item::items[drop.first].name.c_str());
                    ImGui::Text("Production per hour: %f", minion_calculation_data.drops_per_hour[drop.first]);
                    ImGui::Text("Profit per hour (NPC): %f", minion_calculation_data.profits_per_hour[drop.first]);
                    ImGui::Text("Profit per hour (Bazaar): %f",
                                minion_calculation_data.bazaar_profit_per_hour[drop.first]);

                    if (compact_ui) ImGui::EndListBox();

                    else imgui_util::dummy(0, 5);
                }

                for (auto skill: minion_calculation_data.skill_xp_drops)
                {
                    ImGui::Text("%s xp (per hour): %f", to_string(skill.first), skill.second);
                }

                imgui_util::dummy(0, 10);
            } else
            {
                ImGui::Text("Sum Profit per hour (NPC): %f", minion_calculation_data.sum_profit_npc);
                ImGui::Text("Sum Profit per hour (Bazaar): %f", minion_calculation_data.sum_profit_bazaar);
                imgui_util::dummy(0, 10);

                ImGui::Text("Sum profit per day (NPC): %f", minion_calculation_data.sum_profit_npc * 24);
                ImGui::Text("Sum profit per day (Bazaar): %f", minion_calculation_data.sum_profit_bazaar * 24);
            }

            ImGui::EndListBox();


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
    minion_calculator::init_recipes("../data/minion_recipes.json");
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
