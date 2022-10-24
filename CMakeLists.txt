cmake_minimum_required(VERSION 3.0.0)
project(xxx VERSION 0.1.0)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Containers
    ${CMAKE_SOURCE_DIR}/Mutex
    ${CMAKE_SOURCE_DIR}/SmartPointer
    ${CMAKE_SOURCE_DIR}/ScopeGuard
    ${CMAKE_SOURCE_DIR}/Thread    
    ${CMAKE_SOURCE_DIR}/VariableArgumentsTemplate
    ${CMAKE_SOURCE_DIR}/TypeDeclaration
    ${CMAKE_SOURCE_DIR}/Other
    ${CMAKE_SOURCE_DIR}/Template
    ${CMAKE_SOURCE_DIR}/Patterns
    ${CMAKE_SOURCE_DIR}/Patterns/Creational
    ${CMAKE_SOURCE_DIR}/Patterns/Structure
    ${CMAKE_SOURCE_DIR}/Patterns/Behavior
    )

include(CTest)
enable_testing()

set (CMAKE_CXX_STANDARD 20)

file(GLOB src 
    main.cpp
    ${CMAKE_SOURCE_DIR}/Containers/*.cpp
    ${CMAKE_SOURCE_DIR}/Mutex/*.cpp
    ${CMAKE_SOURCE_DIR}/SmartPointer/*.cpp
    ${CMAKE_SOURCE_DIR}/ScopeGuard/*.cpp
    ${CMAKE_SOURCE_DIR}/Thread/*.cpp
    ${CMAKE_SOURCE_DIR}/VariableArgumentsTemplate/*.cpp
    ${CMAKE_SOURCE_DIR}/TypeDeclaration/*.cpp
    ${CMAKE_SOURCE_DIR}/Other/*.cpp    
    ${CMAKE_SOURCE_DIR}/Template/*.cpp        
    ${CMAKE_SOURCE_DIR}/Patterns/*.cpp
    ${CMAKE_SOURCE_DIR}/Patterns/Creational/*.cpp
    ${CMAKE_SOURCE_DIR}/Patterns/Structure/*.cpp
    ${CMAKE_SOURCE_DIR}/Patterns/Behavior/*.cpp
    )

set(src ${src})

add_executable(xxx ${src})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
