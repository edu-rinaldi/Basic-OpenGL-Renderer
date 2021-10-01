cmake_minimum_required(VERSION 3.5)
project(EdGL VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)

# GLEW
add_definitions(-DGLEW_STATIC)
add_subdirectory(dependencies/glew)

# GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(dependencies/glfw)

# GLM
add_subdirectory(dependencies/glm)

# Assimp
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(dependencies/assimp)

# STB
set(STB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb)
add_compile_definitions(STB_IMAGE_IMPLEMENTATION)

file(GLOB_RECURSE CPP_FILES CONFIGURE_DEPENDS src/*.cpp src/*.h src/*.hpp)
add_library(${PROJECT_NAME} ${CPP_FILES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        glm
        libglew_static
        glfw
        ${STB_DIR}
        assimp
    PRIVATE
        
    )

target_link_libraries(${PROJECT_NAME} 
    PUBLIC
        glm
        libglew_static
        glfw
        assimp     
    PRIVATE
    )

