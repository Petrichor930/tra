# Add the sourses
file(GLOB_RECURSE SOURCES  CMAKE_CONFIGURE_DEPENDS "PinyCore.cpp")
#
# Add the library
add_library(App ${SOURCES})
#
# Include the header directories
target_include_directories(App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
