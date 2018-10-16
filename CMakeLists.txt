cmake_minimum_required(VERSION 3.12)
project(piksel)
set(CMAKE_CXX_STANDARD 11)
if(EMSCRIPTEN)
set(CMAKE_CXX_COMPILER em++ PARENT_SCOPE)
set(CMAKE_CXX_FLAGS "-s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_GLFW=3 -s WASM=1 --emrun --preload-file data --shell-file shell.html" PARENT_SCOPE)
set(CMAKE_EXECUTABLE_SUFFIX ".html" PARENT_SCOPE)
else()
set(CMAKE_CXX_COMPILER g++ PARENT_SCOPE)
endif(EMSCRIPTEN)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(MSDFGEN_SOURCES
    lib/msdfgen/msdfgen/core/Bitmap.cpp
    lib/msdfgen/msdfgen/core/Contour.cpp
    lib/msdfgen/msdfgen/core/edge-coloring.cpp
    lib/msdfgen/msdfgen/core/edge-segments.cpp
    lib/msdfgen/msdfgen/core/EdgeHolder.cpp
    lib/msdfgen/msdfgen/core/equation-solver.cpp
    lib/msdfgen/msdfgen/core/msdfgen.cpp
    lib/msdfgen/msdfgen/core/render-sdf.cpp
    lib/msdfgen/msdfgen/core/save-bmp.cpp
    lib/msdfgen/msdfgen/core/shape-description.cpp
    lib/msdfgen/msdfgen/core/Shape.cpp
    lib/msdfgen/msdfgen/core/SignedDistance.cpp
    lib/msdfgen/msdfgen/core/Vector2.cpp
)

add_library(msdfgen STATIC ${MSDFGEN_SOURCES})

set(SOURCES
    src/piksel/baseapp.cpp
    src/piksel/font.cpp
    src/piksel/graphics.cpp
    src/piksel/image.cpp
    src/piksel/rng.cpp
    src/piksel/shader.cpp
    src/piksel/shape.cpp
)
add_library(piksel ${SOURCES})

target_include_directories(piksel PUBLIC
    src/piksel
    lib/glm
    lib/stb/include
    lib/msdfgen
    lib/glad/include
)

if(EMSCRIPTEN)
target_link_libraries(piksel
    msdfgen
)
else()
add_library(glad STATIC lib/glad/src/glad.c)
target_include_directories(glad PRIVATE lib/glad/include)
target_link_libraries(glad ${CMAKE_DL_LIBS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)

target_link_libraries(piksel
    glad
    glfw
    msdfgen
)
endif(EMSCRIPTEN)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shell.html ${CMAKE_BINARY_DIR}/shell.html COPYONLY)

set(DATA_SOURCE_DIR ${CMAKE_SOURCE_DIR}/data)
set(DATA_BINARY_DIR ${CMAKE_BINARY_DIR}/data)
add_custom_target(make_data_directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DATA_SOURCE_DIR}
)
add_custom_target(touch_dummy ALL
    COMMAND ${CMAKE_COMMAND} -E touch ${DATA_SOURCE_DIR}/dummy
    DEPENDS make_data_directory
)
add_custom_target(create_data_symlink ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${DATA_SOURCE_DIR} ${DATA_BINARY_DIR}
    DEPENDS touch_dummy
)
add_dependencies(piksel create_data_symlink)
