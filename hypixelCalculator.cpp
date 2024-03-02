#include <GLFW/glfw3.h>

#include "farming.h"
#include "helper.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "mining.h"
#include "minions/minion_calculator.h"


int main()
{
    
    minion_calculator::init_fuels("../data/minion_fuels.json");
    minion_calculator::init_minions("../data/minions.json");
    minion_calculator::init_items("../data/items.json");
    minion_calculator::init_recipes("../data/item_recipes.json");
    minion_calculator::init_recipes("../data/minion_recipes.json");
    minion_calculator::init_buy_requirements("../data/item_buy_requirements.json");

    
    helper::pause();
    


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
        case 1: mining::calculate_mgs();
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
}
