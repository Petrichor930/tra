# Add the sourses
file(GLOB_RECURSE SOURCES CMAKE_CONFIGURE_DEPENDS "PinyCore.cpp")

# Add the library
add_library(Piny ${SOURCES})

# Include the header directories
target_include_directories(Piny PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Piny PRIVATE App stm32cubemx System Extensions Utils Bsp Soc)
