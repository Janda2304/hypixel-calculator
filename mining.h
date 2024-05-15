#pragma once

class mining
{
public:
    static void calculate_mgs(int mining_speed, int mining_fortune, float pristine, float &final_profit, float &mgs);

    static void show_mgs_menu(bool &show_mgs);

    static int mining_speed;
    static int mining_fortune;
    static float pristine;
};
