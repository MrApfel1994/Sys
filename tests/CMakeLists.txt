cmake_minimum_required(VERSION 3.2)
project(test_sys)

add_executable(test_sys TestsApp.cpp
                        #TestPack.cpp
                        #TestJson.cpp
                        TestSignal.cpp)

target_link_libraries(test_sys sys)