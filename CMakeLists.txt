cmake_minimum_required(VERSION 3.28.3-msvc11)
project(MiniECS LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)
FetchContent_Declare(
        IMGUI
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.90.9
)
FetchContent_MakeAvailable(IMGUI)
FetchContent_GetProperties(IMGUI SOURCE_DIR IMGUI_DIR)
set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(
        IMGUI-SFML
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml
        GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(IMGUI-SFML)

add_executable(main main.cpp
        Entity.cpp
        Entity.h
        EntityManager.cpp
        EntityManager.h
        Game.cpp
        Game.h
        Components.cpp
        Components.h
        Vec2.cpp
        Vec2.h
        Helpers.cpp
        Helpers.h
        ComponentRegistry.cpp
        ComponentRegistry.h)
set(CMAKE_CXX_STANDARD 20)
target_link_libraries(main PRIVATE sfml-graphics ImGui-SFML::ImGui-SFML)
target_compile_features(main PRIVATE cxx_std_20)
