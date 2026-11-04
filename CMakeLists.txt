cmake_minimum_required(VERSION 4.3)
project(Gameboy)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG        main
)

FetchContent_MakeAvailable(SDL3)

include_directories(.)

add_executable(Gameboy
        cart.cpp
        cart.h
        core.h
        cpu.cpp
        cpu.h
        main.cpp
        membus.cpp
        membus.h
        core.cpp
        timer.h
        timer.cpp
        ppu.cpp
        ppu.h
        gfx.h
        gfx.cpp
)

target_link_libraries(Gameboy PRIVATE SDL3::SDL3)

if(WIN32)
    add_custom_command(TARGET Gameboy POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SDL3::SDL3>"
            "$<TARGET_FILE_DIR:Gameboy>")
endif()
