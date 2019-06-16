project(CG2019ICMC)

cmake_minimum_required(VERSION 3.10)

include(ExternalProject)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external")
include( glfw )
include( glm )

find_package(OpenGL REQUIRED)
set(OPENGL_INCLUDE_DIR ${OPENGL_INCLUDE_DIR} CACHE STRING "")
set(OPENGL_LIBRARIES ${OPENGL_LIBRARIES} CACHE STRING "")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glad" "external/glad")
set(GLAD_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/assimp" "external/assimp")
set(ASSIMP_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/include")

set(CG2019ICMC_CPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cpp_src)
add_subdirectory(cpp_src)

##################################################################
# EXECUTAVEIS

# Versao orientado a objetos
add_executable( projeto3CPP projeto_3.cpp )
target_link_libraries( projeto3CPP glad assimp ${GLFW_LIBRARIES} cg2019cpp )
target_include_directories( projeto3CPP PUBLIC 
                            ${CG2019ICMC_CPP_INCLUDE_DIR}
                            ${GLFW_INCLUDE_DIR} ${GLAD_INCLUDES} ${ASSIMP_INCLUDES} )
