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

file(COPY data DESTINATION ${CMAKE_BINARY_DIR})
file(COPY fonts DESTINATION ${CMAKE_BINARY_DIR})

add_executable(hypixel_calculator
        src/buy_requirements.cpp
        src/crafting_recipe.cpp
        src/drop_data.cpp
        src/item.cpp
        src/minion.cpp
        src/minion_fuel.cpp
        src/farming.cpp
        hypixelCalculator.cpp
        src/mining.cpp
        src/minion_calculator.cpp
        src/item_xp_drop.cpp
        src/drop_chance_calculator.cpp
        src/style_config.cpp
        ${IMGUI_SOURCES}
)

target_include_directories(hypixel_calculator PRIVATE
        include/GLFW
        include/imgui
        include/json
        include/hypixel-calculator
)

# Link GLFW
find_package(OpenGL REQUIRED)

if (WIN32)
    find_library(GLFW_LIBRARY NAMES glfw3 HINTS ${CMAKE_SOURCE_DIR}/lib)
    target_link_libraries(hypixel_calculator PRIVATE ${GLFW_LIBRARY} OpenGL::GL gdi32)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    target_link_libraries(hypixel_calculator PRIVATE ${GLFW_LIBRARIES} OpenGL::GL)
endif()
