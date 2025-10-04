add_executable(test_any test_any.c)
target_link_libraries(test_any PRIVATE Any)
target_include_directories(test_any PRIVATE ../include)

add_executable(stress_test stress_test.c)
target_link_libraries(stress_test PRIVATE Any)
target_include_directories(stress_test PRIVATE ../include)

add_executable(compatibility_test compatibility_test.c)
target_link_libraries(compatibility_test PRIVATE Any)
target_include_directories(compatibility_test PRIVATE ../include)

# Leak demonstration (used to validate tracking)
add_executable(leak_demo leak_demo.c)
target_link_libraries(leak_demo PRIVATE Any)
target_include_directories(leak_demo PRIVATE ../include)
