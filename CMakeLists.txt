cmake_minimum_required(VERSION 3.28)
project(hypixel_calculator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)




set(GLFW_DIR "include/GLFW")

file(GLOB IMGUI_SOURCES
        include/imgui/imgui.cpp
        include/imgui/imgui_impl_glfw.cpp
        include/imgui/imgui_impl_opengl3.cpp
        include/imgui/imgui_util.cpp
        include/imgui/imgui_tables.cpp
        include/imgui/imgui_demo.cpp
        include/imgui/imgui_widgets.cpp
        include/imgui/imgui_draw.cpp
)

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
        src/color.hpp
        src/settings_data.hpp
        drop_chance_calculator.cpp
        src/style_config.cpp
        ${IMGUI_SOURCES}
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

target_include_directories(hypixel_calculator PRIVATE
        include/GLFW
        include/imgui
        include/json

)

# Link GLFW
find_package(OpenGL REQUIRED)

if (WIN32)
    find_library(GLFW_LIBRARY NAMES glfw3 HINTS ${CMAKE_SOURCE_DIR}/lib)
else()
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
endif()

target_link_libraries(hypixel_calculator PRIVATE
        ${GLFW_LIBRARY}
        OpenGL::GL
        gdi32
)


