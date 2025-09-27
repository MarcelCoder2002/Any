add_executable(example main.c)
target_link_libraries(example PRIVATE Any)
target_include_directories(example PRIVATE ../include)