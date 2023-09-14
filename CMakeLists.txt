cmake_minimum_required(VERSION 3.26)
project(pong)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_executable(pong main.cpp
        classes/LTimer.cpp
        classes/LTimer.h
        classes/Obj.cpp
        classes/Obj.h
        classes/Rec.cpp
        classes/Rec.h
        classes/Ball.cpp
        classes/Ball.h
        classes/LTexture.cpp
        classes/LTexture.h
        defines.h
        classes/Player.cpp
        classes/Player.h)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

include_directories(pong ${SDL2_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})

target_link_libraries(pong ${SDL2_LIBRARIES} -lSDL2_mixer ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES})