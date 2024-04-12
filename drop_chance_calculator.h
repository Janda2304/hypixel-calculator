#ifndef DROP_CHANCE_CALCULATOR_H
#define DROP_CHANCE_CALCULATOR_H



class drop_chance_calculator
{
    static float magic_find;
    static float default_drop_chance;
    static float drop_chance;
    static float pet_luck;
    static bool pet_calculation;

public:
    static void show_drop_chance_menu(bool& show_drop_chance);
    
};



#endif //DROP_CHANCE_CALCULATOR_H
