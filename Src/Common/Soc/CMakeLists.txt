add_library(Soc INTERFACE)

include(../../sdkconfig.cmake)

if(CONFIG_TARGET STREQUAL "stm32h723")
  set(target_folder "stm32h723")

elseif(CONFIG_TARGET STREQUAL "stm32f407")
  set(target_folder "stm32f407")

else()
  message(FATAL_ERROR "Unset target or set Unknown target")
endif()

message( "Soc_board: ${target_folder}")

# Link directories setup
set(CMAKE_C_LINK_FLAGS " -T \"${CMAKE_CURRENT_LIST_DIR}/link/${target_folder}.ld\"" PARENT_SCOPE)

# includes
target_include_directories(${CMAKE_PROJECT_NAME}
  PRIVATE ${target_folder})
