add_executable(test_ContactForce
    test_ContactForce.cpp
)

target_link_libraries(test_ContactForce PRIVATE ht9045_core)

add_test(NAME ContactForce COMMAND test_ContactForce)
