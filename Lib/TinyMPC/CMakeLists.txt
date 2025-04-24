include (${CMAKE_SOURCE_DIR}/tools/cmake/FindInclude.cmake)

# Add the sourses
file(GLOB_RECURSE SOURCES  CMAKE_CONFIGURE_DEPENDS "*.c" "*.cpp")

# Add the library
add_library(TinyMPC STATIC ${SOURCES})

set_property(TARGET TinyMPC PROPERTY POSITION_INDEPENDENT_CODE ON)

# Include the header directories
FIND_INCLUDE_DIR(HEADERS ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(TinyMPC PUBLIC ${HEADERS})

target_compile_definitions(TinyMPC PRIVATE
    EIGEN_DONT_PARALLELIZE # 禁用并行
    EIGEN_NO_DEBUG # 禁用调试断言
    EIGEN_MPL2_ONLY # 仅使用MPL2许可的组件
)

target_link_libraries(TinyMPC PRIVATE Eigen3)


if(USING_CODEGEN) # Defined in top-level CMakeLists.txt
  # Files that are needed for embedded code generation
  list(APPEND EMBEDDED_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/admm.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/admm.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/tiny_api.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/tiny_api.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/types.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/tiny_api_constants.hpp")

  foreach(f ${EMBEDDED_FILES})
    get_filename_component(fname ${f} NAME)

    set(dest_file "${EMBEDDED_BUILD_TINYMPC_DIR}/${fname}")
    list(APPEND EMBEDDED_BUILD_TINYMPC_FILES "${dest_file}")

    add_custom_command(OUTPUT ${dest_file}
            COMMAND ${CMAKE_COMMAND} -E copy "${f}" "${dest_file}"
            DEPENDS ${f}
            COMMENT "Copying ${fname}")
  endforeach()

  add_custom_target(copy_codegen_tinympc_files DEPENDS ${EMBEDDED_BUILD_TINYMPC_FILES})
  add_dependencies(copy_codegen_files copy_codegen_tinympc_files)
endif(USING_CODEGEN)
