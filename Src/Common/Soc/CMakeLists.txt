add_library(Soc INTERFACE)

include(../../sdkconfig.cmake)

if(CONFIG_TARGET STREQUAL "stm32h723")
  set(target_folder "stm32h723")
elseif(CONFIG_TARGET STREQUAL "stm32f407")
  set(target_folder "stm32f407")
elseif(CONFIG_TARGET STREQUAL "stm32f446")
  set(target_folder "stm32f446")
elseif(CONFIG_TARGET STREQUAL "stm32g473")
  set(target_folder "stm32fg473")
else()
  message(FATAL_ERROR "Unset target or set Unknown target")
endif()

set(soc "${CMAKE_CURRENT_SOURCE_DIR}/${target_folder}/" PARENT_SCOPE)
# includes
target_include_directories(Soc
  INTERFACE ${target_folder})

# Link directories setup
set(CMAKE_C_LINK_FLAGS " -T
\"${CMAKE_CURRENT_LIST_DIR}/Link/${target_folder}.ld\"" PARENT_SCOPE)


