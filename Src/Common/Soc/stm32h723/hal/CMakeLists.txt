include (${CMAKE_SOURCE_DIR}/tools/cmake/FindInclude.cmake)

# Add the sourses
file(GLOB_RECURSE SOURCES CMAKE_CONFIGURE_DEPENDS "*.c" "*.cpp" "*.s")
list(REMOVE_ITEM SOURCES "systemcall.c sysmem.c")
file(GLOB_RECURSE SYSTEM CMAKE_CONFIGURE_DEPENDS "syscalls.c" "sysmem.c")

# Add the library
add_library(stm32cubemx STATIC ${SOURCES})
add_library(systemcall OBJECT ${SYSTEM})

# Include the header directories
FIND_INCLUDE_DIR(HEADERS ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(stm32cubemx PUBLIC ${HEADERS})

# link
target_link_libraries(stm32cubemx PRIVATE App)
