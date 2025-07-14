cmake_minimum_required(VERSION 3.10)
project(3dOctreeCompression)

set(CMAKE_CXX_STANDARD 17)

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Add executable
add_executable(3dOctreeCompression 
    main.cpp
    src/glad.c
)

# Include directories
target_include_directories(3dOctreeCompression PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(3dOctreeCompression 
    OpenGL::GL
    glfw
    glm::glm
)
