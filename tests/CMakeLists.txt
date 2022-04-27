
# testRegex

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


# testFileSync

add_executable(testFileSync
    testFileSync.cpp
)

target_link_libraries(testFileSync
    PRIVATE FileSyncLibrary
)

add_test(
    NAME testFileSync
    COMMAND testFileSync
)
