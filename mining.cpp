#include "mining.h"
#include <iostream>
#include <cstdlib>

#include "helper.h"

void mining::calculate_mgs()
{
    int mining_speed;
    int mining_fortune;
    float pristine;

    
    
    system("cls");
    std::cout << "Enter your mining speed:\n";
    std::cin >> mining_speed;
    
    std::cout << "Enter your mining fortune:\n";
    std::cin >> mining_fortune;
    
    std::cout << "Enter your pristine:\n";
    std::cin >> pristine;

    float mgs = mining_speed / 100 * (1 + mining_fortune / 100) * (1 + pristine * 0.79);
    
    std::cout << "Your Mining Gear Score (MGS) is: " << mgs << "\n";
    
    float final_profit = mgs * 1080;
    std::cout << "You will make approximately " << final_profit << " coins a hour\n";
    helper::pause();
}
