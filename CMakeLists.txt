cmake_minimum_required(VERSION 3.10)

project(custom_compiler LANGUAGES C)

# Enable debug symbols if not already set externally
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(main main.c ./parser/parser.c ./lexer/lexer.c ./scope/stack.c  "./symbol table/hashmap.c" "./code generator/code_generator.c")
