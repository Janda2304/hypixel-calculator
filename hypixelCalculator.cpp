#include <GLFW/glfw3.h>

#include "farming.h"
#include "helper.hpp"
#include "imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "mining.h"
#include "imgui/imgui_util.h"
#include "minions/minion_calculator.h"
#include "src/minion_calculation_data.h"

bool show_mgs = false;
bool show_farming = false;
bool show_minion_profit = false;
minion_calculation_data minion_calculation_data;
bool minion_simple_calculation = false;









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
    // Cleanup
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
        ImVec2 win_pos_offset = ImVec2(static_cast<float>(win_width - fb_width) / 2.0f, static_cast<float>(win_height - fb_height));
        
        ImGui::SetNextWindowPos(win_pos_offset);
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(fb_width), static_cast<float>(fb_height)));
        ImGui::Begin("Hypixel Calculator", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
        
        imgui_util::change_text_color(1, 1, 1, 1);
        imgui_util::change_background_color(0.071f, 0.071f, 0.071f, 1);
        imgui_util::change_button_hover_color(0.2f, 0.2f, 0.2f, 0.5f);
        imgui_util::change_button_color(0, 0, 0, 0.5f);
        imgui_util::change_frame_background_color(0.25f, 0.25f, 0.25f, 0.5f);
        imgui_util::change_item_rounding(5);
        imgui_util::change_button_clicked_color(0,0,0,0.75f);

        

        if (!show_mgs && !show_farming && !show_minion_profit)
        {
            ImVec2 screenSize = ImGui::GetIO().DisplaySize;
            ImVec2 buttonSize(400, 50); 
            ImVec2 buttonPos((screenSize.x - buttonSize.x) * 0.5f, (screenSize.y - buttonSize.y) * 0.35f);
            ImGui::SetCursorPos(buttonPos);

            //apply styles
            imgui_util::change_item_spacing_y(10);
           
            
            ImGui::BeginListBox("##Main Menu", ImVec2(410, 300));
            
            if (ImGui::Button("Calculate MGS (Mining Gear Score)", ImVec2(400, 50)))
            {
                show_mgs = true;
            }

            if (ImGui::Button("Calculate Farming Profits", ImVec2(400, 50)))
            {
                show_farming = true;
            }

            if (ImGui::Button("Calculate Minion Profit", ImVec2(400, 50)))
            {
                show_minion_profit = true;
            }

            if (ImGui::Button("Calculate Best Minion", ImVec2(400, 50)))
            {
            }

            if (ImGui::Button("Exit", ImVec2(400, 50)))
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

        if (show_minion_profit)
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.5f));
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

            // Search bar
            ImGui::BeginListBox("##Minion Selection", ImVec2(305, 230));
            
            ImGui::PushItemWidth(300);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 5));

            
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
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); //reset spacing

            
            ImGui::EndListBox();

            ImGui::Text("Current Minion: %s", minion_names[minion_calculator::selected_minion_index]);
            ImGui::EndListBox();
            
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 0));
            ImGui::SameLine();
           
            
            ImGui::BeginListBox("##Fuel Selection", ImVec2(305, 230));
            
            ImGui::PushItemWidth(300);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 5));

            
            ImGui::InputText("Search Fuels", fuel_filter_buffer, IM_ARRAYSIZE(fuel_filter_buffer));
            ImGui::BeginListBox("##Fuels");
            

            for (size_t i = 0; i < fuel_names.size(); ++i)
            {
                // Convert the fuel name to lowercase
                std::string fuel_name_lowercase = fuel_names[i];
                std::transform(fuel_name_lowercase.begin(), fuel_name_lowercase.end(), fuel_name_lowercase.begin(), tolower);

                std::string fuel_filter_lowercase = fuel_filter_buffer;
                std::transform(fuel_filter_lowercase.begin(), fuel_filter_lowercase.end(), fuel_filter_lowercase.begin(), tolower);

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


            ImGui::Dummy(ImVec2(0, 10));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 5));

            ImGui::Checkbox("Diamond Spreading", &minion_calculator::diamond_spreading);

            ImGui::Checkbox("Simple Calculation", &minion_simple_calculation);

            ImGui::PushItemWidth(155);
            ImGui::InputInt("Storage Capacity (stacks)", &minion_calculator::storage_capacity, 0, 0, ImGuiInputTextFlags_CharsDecimal);

            ImGui::SameLine();
            ImGui::Dummy(ImVec2(10, 0));
            ImGui::SameLine();

            ImGui::Text("Storage Capacity (items): %d", minion_calculator::storage_capacity * 64);
            ImGui::InputInt("Other Percentage Boosts", &minion_calculator::other_boosts_percentage, 0, 0, ImGuiInputTextFlags_CharsDecimal);
    

            if (ImGui::Button("Calculate", ImVec2(400, 40)))
            {
                minion_calculator::selected_fuel_id = fuel_ids[minion_calculator::selected_fuel_index];
                minion_calculator::selected_minion_id = minion_ids[minion_calculator::selected_minion_index];

                minion_calculator::calc_minion_profit(minion::minions[minion_calculator::selected_minion_id],
                                                      minion_calculation_data,
                                                      minion_fuel::minion_fuels[minion_calculator::selected_fuel_id],
                                                      minion_calculator::diamond_spreading);
            }


            ImGui::Dummy(ImVec2(0, 5));
            ImGui::BeginListBox("##Minion Info", ImVec2(400, 200));


            ImGui::Text("Minion Production Rate: %f s", minion_calculation_data.minion_production_rate);
            ImGui::Dummy(ImVec2(0, 10));
            if (!minion_simple_calculation)
            {
                ImGui::Text("DROPS:");
           
                for (const auto &drop: minion_calculation_data.minion_drops)
                {
                    ImGui::Text("%s : ", item::items[drop.first].name.c_str());
                    ImGui::Text("Production per hour: %f", minion_calculation_data.drops_per_hour[drop.first]);
                    ImGui::Text("Profit per hour (NPC): %f", minion_calculation_data.profits_per_hour[drop.first]);
                    ImGui::Text("Profit per hour (Bazaar): %f", minion_calculation_data.bazaar_profit_per_hour[drop.first]);
                    ImGui::Dummy(ImVec2(0, 10));
                }

                for (auto skill: minion_calculation_data.skill_xp_drops)
                {
                    ImGui::Text("%s xp (per hour): %f", to_string(skill.first), skill.second);
                }

                ImGui::Dummy(ImVec2(0, 10));
            } else
            {
                ImGui::Text("Sum Profit per hour (NPC): %f", minion_calculation_data.sum_profit_npc);
                ImGui::Text("Sum Profit per hour (Bazaar): %f", minion_calculation_data.sum_profit_bazaar);
                ImGui::Dummy(ImVec2(0, 10));

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
