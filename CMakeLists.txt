cmake_minimum_required(VERSION 3.5)

project(FileSync LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(FileSyncLibrary
    FileSync.cpp
    FileSync.hpp
    RegularExpressionHelpers.cpp
    RegularExpressionHelpers.hpp
)

target_include_directories(FileSyncLibrary
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)


add_executable(FileSync
    main.cpp
)

target_link_libraries(FileSync
    PRIVATE FileSyncLibrary
)

add_subdirectory(tests)
