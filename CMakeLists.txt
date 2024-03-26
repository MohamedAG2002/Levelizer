cmake_minimum_required(VERSION 3.27)
project(levelizer)

# Variables
############################################################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LVL_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(LVL_HEADER_DIR ${CMAKE_SOURCE_DIR}/include/)
############################################################

# Project sources
############################################################
set(LVL_SOURCES 
  ${LVL_SRC_DIR}/lexer.cpp
  ${LVL_SRC_DIR}/parser.cpp
  ${LVL_SRC_DIR}/level.cpp
  ${LVL_SRC_DIR}/entity.cpp
)
############################################################

# Compiling and linking
############################################################
add_library(${PROJECT_NAME} ${CMAKE_BUILD_TYPE} ${LVL_SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -lm)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_include_directories(${PROJECT_NAME} BEFORE PUBLIC ${LVL_HEADER_DIR}/levelizer)

# Install the library in the correct place
install(TARGETS ${PROJECT_NAME} 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib)
install(DIRECTORY ${LVL_HEADER_DIR} DESTINATION include)
############################################################
