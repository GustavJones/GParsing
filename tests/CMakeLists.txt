add_executable(TEST1 test1.cpp)
target_include_directories(TEST1 PRIVATE ../include/)
target_link_libraries(TEST1 PRIVATE GParsing)

add_executable(TEST2 test2.cpp)
target_include_directories(TEST2 PRIVATE ../include/)
target_link_libraries(TEST2 PRIVATE GParsing)
