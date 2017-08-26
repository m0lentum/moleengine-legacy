#modified from the SFML wiki

cmake_minimum_required(VERSION 2.6)

# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

project(MoleEngine)

# Define sources and library
set(LIBRARY_NAME "LibMoleEngine")
file(GLOB_RECURSE SOURCES src/ME/*.cpp)
file(GLOB_RECURSE HPP include/ME/*.hpp)
file(GLOB_RECURSE INL include/ME/*.inl)
list(APPEND HEADERS ${HPP} ${INL})
include_directories(include/ME)
add_library(${LIBRARY_NAME} STATIC ${SOURCES})

set(SFML_ROOT "" CACHE PATH "Specify your SFML install folder here if not found automatically")
# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2.4 REQUIRED network audio graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${LIBRARY_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
  #from Stack Overflow: expose includes to other projects
  set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include ${SFML_INCLUDE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
endif()

# Install library
install(TARGETS ${LIBRARY_NAME} DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)