#include <GLFW/glfw3.h>

#include "farming.h"
#include "helper.hpp"
#include "imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "mining.h"
#include "minions/minion_calculator.h"

bool show_mgs = false;
bool show_farming = false;
bool show_minion_profit = false;

void ImGuiInit(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    io.Fonts->AddFontFromFileTTF("../fonts/Roboto-Regular.ttf", 18.0f);

    ImGui::StyleColorsDark();
}

void ImGuiShutdown()
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
void renderLoop(GLFWwindow* window)
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

        // Start ImGui window
        ImGui::SetNextWindowPos(win_pos_offset);
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(fb_width), static_cast<float>(fb_height))); // Set window size to framebuffer size
        ImGui::Begin("Hypixel Calculator", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);


     
       
        
        if (!show_mgs && !show_farming && !show_minion_profit)
        {
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
        }

        mining::show_mgs_menu(show_mgs);
        farming::show_farming_menu(show_farming);

        if (show_minion_profit)
        {
            std::string current_minion_id;
            std::vector<const char*> minion_names;

            // Populate the vector of minion names
            for (const auto& minion : minion::minions)
            {
                minion_names.push_back(minion.second.name.c_str());
            }

            static char filter_buffer[256] = "";

            // Search bar
            ImGui::PushItemWidth(300);
            ImGui::InputText("Search Minions", filter_buffer, IM_ARRAYSIZE(filter_buffer));

           
            ImGui::BeginListBox("##Minions");

            for (size_t i = 0; i < minion_names.size(); ++i)
            {
                if (filter_buffer[0] == '\0' || strstr(minion_names[i], filter_buffer) != nullptr) // Apply filter
                {
                    bool is_selected = (minion_calculator::selected_minion_id_index == static_cast<int>(i));
                    
                    if (ImGui::Selectable(minion_names[i], is_selected)) minion_calculator::selected_minion_id_index = static_cast<int>(i);

                    if (is_selected) ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndListBox();

            // Display the selected minion
            ImGui::Text("Current Minion: %s", minion_names[minion_calculator::selected_minion_id_index]);

            
            ImGui::Checkbox("Diamond Spreading", &minion_calculator::diamond_spreading);

            if (ImGui::Button("Calculate", ImVec2(400, 50)))
            {
                float profit;
                float bazaar_profit;
                minion_fuel fuel;
                minion_calculator::calc_minion_profit(minion::minions[current_minion_id], profit, bazaar_profit, fuel, minion_calculator::diamond_spreading);
            }

          


            if (ImGui::Button("Back to main menu", ImVec2(400, 50)))
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
    ImGuiShutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
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
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Multipurpose Hypixel Skyblock Calculator", nullptr, nullptr);
    
    if (!window) return -1;
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    ImGuiInit(window);
    
    renderLoop(window);

    
    while (true)
    {
        int choice;
        helper::clear();
        std::cout << "1)calculate MGS\n2)calculate farming profits\n3)calculate minion profit\n4)calculate best minion\n5)end program\n";
        std::cin >> choice;

        std::string minion_id;
        float profit;
        minion_fuel fuel;

        switch (choice)
        {
        case 1: 
            break;

        case 2: 
            break;

        case 3:
            helper::clear();
            std::cout << "Enter your minion id:\n";
            std::cin >> minion_id;

            helper::clear();
            minion_calculator::calc_minion_profit(minion::minions[minion_id], profit, profit, fuel);
            helper::pause();
            break;

        case 4:
            minion_calculator::calc_best_minion();
            helper::pause();
            break;

        default: return 0;
        }
    }
}
