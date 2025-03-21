cmake_minimum_required(VERSION 3.28.1)
project(plastic CXX)

set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_FLAGS "-O3 -mavx512f -mavx512dq -mavx512bw -mavx512vbmi -mavx512vbmi2 -mavx512vl")
set(CMAKE_CXX_FLAGS "-O0 -g -DDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_BUILD_TYPE=Debug)


set(VERT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/shader.vert")
set(FRAG_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/shader.frag")
set(COMP_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/shader.comp")

set(VERT_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/shader.vert.spv")
set(FRAG_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/shader.frag.spv")
set(COMP_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/shader.comp.spv")
add_custom_command( 
	OUTPUT ${VERT_OUTPUT} ${FRAG_OUTPUT} ${COMP_OUTPUT}
	# COMMAND glslc -fshader-stage=vert -o ${VERT_OUTPUT} ${VERT_SOURCE}\; glslc -fshader-stage=frag -o ${FRAG_OUTPUT} ${FRAG_SOURCE}
	COMMAND glslc -o ${VERT_OUTPUT} ${VERT_SOURCE}\; glslc -o ${FRAG_OUTPUT} ${FRAG_SOURCE}\; glslc -o ${COMP_OUTPUT} ${COMP_SOURCE}
	DEPENDS ${VERT_SOURCE} ${FRAG_SOURCE} ${COMP_SOURCE}
)
add_custom_target(shader_target DEPENDS ${VERT_OUTPUT} ${FRAG_OUTPUT})

add_subdirectory("glfw")
add_subdirectory("glm")
add_subdirectory("Vulkan-Headers")
add_subdirectory("Vulkan-Utility-Libraries")

add_executable(plastic)
# target_link_options(plastic PRIVATE -fuse-ld=mold)
target_precompile_headers(plastic PUBLIC includes.h)
target_sources(plastic PRIVATE main.cpp)


target_link_libraries(plastic PRIVATE Vulkan::Headers glfw glm::glm-header-only Vulkan::UtilityHeaders)
add_dependencies(plastic shader_target)

