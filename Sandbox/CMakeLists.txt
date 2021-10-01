cmake_minimum_required(VERSION 3.8)
project(Sandbox VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp src/*.h src/*.hpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} EdGL)
target_include_directories(${PROJECT_NAME} PRIVATE EdGL)

set (source ${CMAKE_CURRENT_SOURCE_DIR}/res)
set (destination $<TARGET_FILE_DIR:Sandbox>/res)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination} 
    DEPENDS ${destination}
    COMMENT "symbolic link resources folder from ${source} => ${destination}"
)

if(MSVC)
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Sandbox>)
endif(MSVC)