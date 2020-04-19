cmake_minimum_required(VERSION 3.10)
project(komodo-doxybook2)

# Set build type
if(NOT CMAKE_BUILD_TYPE) 
  set(CMAKE_BUILD_TYPE "Release") 
endif()
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Select build type") 
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")

# Add the DoxyBook2 library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodules/doxybook2)

# Add the Antara Gaming SDK
# This is probably not needed if you just want to generate the documentation.
# Adding this would force CMake to compile the entire SDK
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodules/antara-gaming-sdk)

# Add custom sources
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/my_src)


