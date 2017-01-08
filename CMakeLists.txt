cmake_minimum_required(VERSION 2.8)

project(TobyG)

set(CMAKE_C_FLAGS "-g -Wall -Wextra -Werror -ansi -pedantic -pedantic-errors")

add_library(tobyg SHARED
    src/io/file.c
    src/io/path.c
    src/game/gamemgr.c
    src/gfx/gfx.c
    src/gfx/mesh.c
    src/gfx/meshbatch.c
    src/gfx/gl.c
    src/gfx/instance.c
    src/gfx/camera.c
    src/gfx/texture.c
    src/gfx/math.c
    src/gfx/renderer.c)
target_link_libraries(tobyg SDL2 GLEW webp)
