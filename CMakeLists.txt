cmake_minimum_required(VERSION 3.28)
project(hypixel_calculator)

set(CMAKE_CXX_STANDARD 17)

add_executable(hypixel_calculator
        src/buy_requirements.cpp
        src/buy_requirements.h
        src/crafting_recipe.cpp
        src/crafting_recipe.h
        src/drop_data.cpp
        src/drop_data.h
        src/item.cpp
        src/item.h
        src/minion.cpp
        src/minion.h
        src/minion_fuel.cpp
        src/minion_fuel.h
        nlohmann/json.hpp
        nlohmann/json_fwd.hpp
        farming.cpp
        farming.h
        helper.hpp
        hypixelCalculator.cpp
        mining.cpp
        mining.h
        minions/minion_calculator.cpp
        minions/minion_calculator.h)



