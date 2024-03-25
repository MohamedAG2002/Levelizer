cmake_minimum_required(VERSION 3.27)
project(levelizer)

# Variables
############################################################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set(LVL_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
############################################################

# Project sources
############################################################
set(LVL_SOURCES 
  ${LVL_SRC_DIR}/main.cpp
  ${LVL_SRC_DIR}/lexer.cpp
  ${LVL_SRC_DIR}/parser.cpp
  ${LVL_SRC_DIR}/level.cpp
  ${LVL_SRC_DIR}/entity.cpp
)
############################################################

# Compiling and linking
############################################################
add_executable(${PROJECT_NAME} ${LVL_SOURCES})

file(COPY levels DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_compile_options(${PROJECT_NAME} PRIVATE -lm)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_include_directories(${PROJECT_NAME} BEFORE PUBLIC ${LVL_SRC_DIR})
############################################################
