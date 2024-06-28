cmake_minimum_required(VERSION 3.28)
project(LearningCPP)

set(CMAKE_CXX_STANDARD 23)

# Set the build type to Release explicitly
set(CMAKE_BUILD_TYPE Release)

# Add optimization flags for Release build
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -mtune=native -funroll-loops -ffast-math -flto")

# Find SFML package
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add the executable and specify source files
add_executable(LearningCPP
        src/main.cpp
        src/MandelBrot.cpp
        src/MandelBrot.h
        src/Display.cpp
        src/Display.h
        src/CoordPlane.cpp
        src/CoordPlane.h
)

# Link SFML libraries
target_link_libraries(LearningCPP PRIVATE sfml-graphics sfml-window sfml-system)

# Include SFML directories
target_include_directories(LearningCPP PRIVATE ${SFML_INCLUDE_DIR})
