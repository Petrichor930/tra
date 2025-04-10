include (${CMAKE_SOURCE_DIR}/tools/cmake/FindInclude.cmake)

# Add the sourses
file(GLOB_RECURSE SOURCES CMAKE_CONFIGURE_DEPENDS "*.c" "*.cpp" "*.s")

# Add the library
add_library(stm32cubemx OBJECT ${SOURCES})

# Include the header directories
FIND_INCLUDE_DIR(HEADERS ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(stm32cubemx PUBLIC ${HEADERS})

# compile
target_compile_definitions(stm32cubemx PUBLIC
  USE_HAL_DRIVER
  STM32H723xx
  ARM_MATH_CM7
  ARM_MATH_MATRIX_CHECK
  ARM_MATH_ROUNDING
  ARM_MATH_LOOPUNROLL
  DISABLEFLOAT16
)
