#include <GLFW/glfw3.h>

#include "farming.h"
#include "helper.hpp"
#include "imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "mining.h"
#include "minions/minion_calculator.h"

bool show_mgs = false;
char input[256];
std::string input_string;

void ImGuiInit(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    io.Fonts->AddFontFromFileTTF("fonts/Roboto-Regular.ttf", 18.0f);
    ImGui::StyleColorsDark();
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


     
       
        
        // Add your GUI elements here
        if (ImGui::Button("Calculate MGS (Mining Gear Score)", ImVec2(400, 50)) && !show_mgs)
        {
            show_mgs = !show_mgs;
            //mining::calculate_mgs();
            std::cout << input << std::endl;
        }
       
        if(ImGui::Button("Calculate Farming Profits", ImVec2(400, 50)))
        {
            
        }
        
        if(ImGui::Button("Calculate Minion Profit", ImVec2(400, 50)))
        {
            
        }
            
        if (ImGui::Button("Calculate Best Minion", ImVec2(400, 50)))
        {
            
        }

        if (show_mgs)
        {
            ImGui::InputInt("Mining Speed", &mining::mining_speed);
            ImGui::Text(input);
        }

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

        case 2: farming::calculate_farming_profit();
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
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}
