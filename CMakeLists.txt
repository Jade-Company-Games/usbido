cmake_minimum_required(VERSION 3.20)
project(usbido VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
add_subdirectory(vendor/SDL)

add_library(imgui STATIC
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_tables.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/backends/imgui_impl_sdl3.cpp
        vendor/imgui/backends/imgui_impl_sdlrenderer3.cpp
)
target_include_directories(imgui PUBLIC
        vendor/imgui
        vendor/imgui/backends
        vendor/SDL/include
)
target_link_libraries(imgui PUBLIC SDL3-static)

add_executable(usbido src/main.cpp)
target_include_directories(usbido PRIVATE
        vendor/SDL/include
        vendor/imgui
        vendor/imgui/backends
)
target_link_libraries(usbido PRIVATE imgui SDL3-static)

add_executable(usbido-monitor src/monitor/main.cpp)
target_include_directories(usbido-monitor PRIVATE
        vendor/SDL/include
)
target_link_libraries(usbido-monitor PRIVATE SDL3-static)