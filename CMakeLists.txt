cmake_minimum_required(VERSION 3.25)
project(testTopSystems)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(SDL2_PATH "SDL2-2.28.3\\x86_64-w64-mingw32")
set(SDL2_ttf_PATH "SDL2_ttf\\x86_64-w64-mingw32")
set(SDL2_image_PATH "SDL2_image\\x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_ttf_INCLUDE_DIR})
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_image_INCLUDE_DIR})

add_executable(testTopSystems main.cpp app/app.cpp app/settings.cpp object/camera.cpp app/priority_tree.cpp util/pugixml.cpp object/field.cpp util/transformation.cpp media/render_manager.cpp media/resource_manager.cpp media/sprite.cpp object/dynamic_object.cpp media/animation.cpp object/position.cpp object/static_object.cpp app/path_searching.cpp app/simulation.cpp)

target_link_libraries(testTopSystems ${SDL2_LIBRARY})
target_link_libraries(testTopSystems ${SDL2_ttf_LIBRARY})
target_link_libraries(testTopSystems ${SDL2_image_LIBRARY})