add_library(Soc INTERFACE)

include(../../sdkconfig.cmake)

if(CONFIG_TARGET STREQUAL "stm32h723")
  set(target_folder "stm32h723")
  set(cortex "cortex-m7" PARENT_SCOPE)
elseif(CONFIG_TARGET STREQUAL "stm32f407")
  set(target_folder "stm32f407")
  set(cortex "cortex-m4" PARENT_SCOPE)
elseif(CONFIG_TARGET STREQUAL "stm32f446")
  set(target_folder "stm32f446" )
  set(cortex "cortex-m4" PARENT_SCOPE)
elseif(CONFIG_TARGET STREQUAL "stm32g473")
  set(target_folder "stm32fg473")
  set(cortex "cortex-m4" PARENT_SCOPE)
else()
  message(FATAL_ERROR "Unset target or set Unknown target")
endif()

message( "soc board: ${target_folder}" )
set(soc_folder "${CMAKE_CURRENT_SOURCE_DIR}/${target_folder}/" PARENT_SCOPE)

# includes
target_include_directories(Soc
  INTERFACE ${target_folder})




