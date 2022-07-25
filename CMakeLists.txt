cmake_minimum_required(VERSION 3.0.0)
project(xxx VERSION 0.1.0)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Mutex
    ${CMAKE_SOURCE_DIR}/SmartPointer
    ${CMAKE_SOURCE_DIR}/ScopeGuard
    ${CMAKE_SOURCE_DIR}/Thread    
    ${CMAKE_SOURCE_DIR}/VariableArgumentsTemplate
    ${CMAKE_SOURCE_DIR}/TypeDeclaration
    )

include(CTest)
enable_testing()

set(src 
    main.cpp
    ${CMAKE_SOURCE_DIR}/Mutex/mutex_test.cpp
    ${CMAKE_SOURCE_DIR}/SmartPointer/smartpointer_test.cpp
    ${CMAKE_SOURCE_DIR}/ScopeGuard/scopeguard_test.cpp
    ${CMAKE_SOURCE_DIR}/Thread/thread_test.cpp
    ${CMAKE_SOURCE_DIR}/Thread/ThreadsPool.cpp
    ${CMAKE_SOURCE_DIR}/VariableArgumentsTemplate/vatemplate_test.cpp
    ${CMAKE_SOURCE_DIR}/TypeDeclaration/typedeclaration_test.cpp
    )


add_executable(xxx ${src})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
