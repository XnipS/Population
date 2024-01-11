# Infomation
cmake_minimum_required(VERSION 3.5)
project(Population LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Glob files
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.h)
# Get imgui path
set(IMGUI_PATH depend/imgui)
# Get SDL2 from package manager
find_package(SDL2 REQUIRED)
# Glob imgui
file(GLOB IMGUI_GLOB
    ${IMGUI_PATH}/*.cpp
    ${IMGUI_PATH}/*.h
    # Specific bindings
    ${IMGUI_PATH}/backends/imgui_impl_sdl2.cpp
	${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
)
# Imgui include
INCLUDE_DIRECTORIES(
    ${IMGUI_PATH}
	${SDL2_INCLUDE_DIRS}
)
# BUILD imgui
add_library(imgui STATIC ${IMGUI_GLOB})
target_link_libraries(imgui SDL2)
target_include_directories(imgui PUBLIC SDL2 SDL2main)
# BUILD FluidisedBed Lib
add_library(libPopulation SHARED
${SOURCE_FILES}
${HEADER_FILES}
)
target_include_directories(libPopulation PRIVATE imgui ${SDL2_INCLUDE_DIRS} ${VM_PATH})
# BUILD Main Simulator Executable
add_executable(Population src/main.cpp)
target_include_directories(Population PRIVATE ${SDL2_INCLUDE_DIRS} ${VM_PATH})
target_link_libraries(Population PUBLIC SDL2 libPopulation imgui OpenGL)
