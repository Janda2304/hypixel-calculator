cmake_minimum_required(VERSION 3.28)
project(hypixel_calculator)

set(CMAKE_CXX_STANDARD 17)

add_executable(hypixel_calculator
        global_src/buy_requirements.cpp
        global_src/buy_requirements.h
        global_src/crafting_recipe.cpp
        global_src/crafting_recipe.h
        global_src/drop_data.cpp
        global_src/drop_data.h
        global_src/item.cpp
        global_src/item.h
        global_src/minion.cpp
        global_src/minion.h
        global_src/minion_fuel.cpp
        global_src/minion_fuel.h
        single_include/nlohmann/json.hpp
        single_include/nlohmann/json_fwd.hpp
        farming.cpp
        farming.h
        helper.h
        hypixelCalculator.cpp
        mining.cpp
        mining.h
        minion_calculator.cpp
        minion_calculator.h)



