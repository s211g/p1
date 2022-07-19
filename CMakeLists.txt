cmake_minimum_required(VERSION 3.0.0)
project(xxx VERSION 0.1.0)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/mutex
    )

include(CTest)
enable_testing()

set(src 
    main.cpp
    ${CMAKE_SOURCE_DIR}/mutex/mutex_test.cpp
)

add_executable(xxx ${src})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
