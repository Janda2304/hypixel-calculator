cmake_minimum_required(VERSION 3.28)
project(hypixel_calculator)

set(CMAKE_CXX_STANDARD 17)

add_executable(hypixel_calculator
        src/buy_requirements.cpp
        src/crafting_recipe.cpp
        src/drop_data.cpp
        src/item.cpp
        src/minion.cpp
        src/minion_fuel.cpp
        farming.cpp
        hypixelCalculator.cpp
        mining.cpp
        minions/minion_calculator.cpp
        src/item_xp_drop.cpp
        imgui/imgui_impl_glfw.cpp
        imgui/imgui_impl_opengl3.cpp
        imgui/imgui_util.cpp
        src/color.hpp
        src/settings_data.hpp
        drop_chance_calculator.cpp
        drop_chance_calculator.h
        src/style_config.cpp
        src/style_config.h
)




find_package(imgui CONFIG REQUIRED)
target_link_libraries(hypixel_calculator PRIVATE imgui::imgui)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(hypixel_calculator PRIVATE glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(hypixel_calculator PRIVATE OpenGL::GL)


