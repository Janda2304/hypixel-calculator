#include "mining.h"
#include <iostream>
#include <cstdlib>

#include "helper.hpp"

int mining::mining_speed;
int mining::mining_fortune;
float mining::pristine;




void mining::calculate_mgs(int mining_speed, int mining_fortune, float pristine, float& final_profit, float& mgs)
{
    mgs = mining_speed / 100 * (1 + mining_fortune / 100) * (1 + pristine * 0.79);
    final_profit = mgs * 1080;
}
