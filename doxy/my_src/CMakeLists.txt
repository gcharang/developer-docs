# Find the sources 
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

# Add the project executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link Doxybook2
# The "Doxybook2" is the name of the CMake target!
# target is defined by add_library or add_executable.
# Don't explicitly link libxyz.a 
target_link_libraries(${PROJECT_NAME} Doxybook2)

