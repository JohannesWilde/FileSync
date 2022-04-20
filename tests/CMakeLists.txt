
add_executable(testRegex
    testRegex.cpp
)

target_link_libraries(testRegex
    PRIVATE FileSyncLibrary
)


add_test(
    NAME testRegex
    COMMAND testRegex
)
