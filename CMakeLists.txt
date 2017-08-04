#taken from the SFML wiki and modified to create a library instead of an executable

cmake_minimum_required(VERSION 2.6)

# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

project(MoleEngine)

# Define sources and library
set(LIBRARY_NAME "LibMoleEngine")
set(SOURCES)
set(HEADERS)
add_subdirectory(src/ME)
add_subdirectory(include/ME)
include_directories(include/ME)
add_library(${LIBRARY_NAME} ${SOURCES} ${HEADERS})

set(CMAKE_PREFIX_PATH "" CACHE PATH "Specify your SFML install folder here if not found automatically")

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${LIBRARY_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

# Install library
install(TARGETS ${LIBRARY_NAME} DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)