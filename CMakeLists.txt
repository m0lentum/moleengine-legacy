#modified from the SFML wiki

cmake_minimum_required(VERSION 2.6)

# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

project(MoleEngine)

# Define sources and library
set(LIBRARY_NAME "MoleEngine")
file(GLOB_RECURSE SOURCES src/ME/*.cpp)
file(GLOB_RECURSE IMGUI_SRC src/imgui/*.cpp)
include_directories(include/ME include/imgui)
add_library(${LIBRARY_NAME} STATIC ${SOURCES} ${IMGUI_SRC})

set(SFML_ROOT "" CACHE PATH "Specify your SFML install folder here if not found automatically")
# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2.4 REQUIRED network audio graphics window system)
find_package(OpenGL REQUIRED)

include_directories(${SFML_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS})

target_link_libraries(${LIBRARY_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${OPENGL_LIBRARIES})
#from Stack Overflow: expose includes to other projects
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include ${SFML_INCLUDE_DIR}
CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)



# Install library
install(TARGETS ${LIBRARY_NAME} DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)