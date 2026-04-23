cmake_minimum_required(VERSION 3.5)

project(2D-Rigid-Body)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# run (./build/main)
add_custom_target(run COMMAND ${CMAKE_BINARY_DIR}/Debug/main DEPENDS main)
add_custom_target(debug COMMAND lldb ${CMAKE_BINARY_DIR}/Debug/main DEPENDS main)

# for now this is our simple main.cpp -> main executable target
add_executable(main main.cpp)

# Link to the actual SDL3 library.
target_link_libraries(main PRIVATE SDL3::SDL3)
