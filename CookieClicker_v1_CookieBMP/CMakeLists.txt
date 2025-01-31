cmake_minimum_required(VERSION 3.27)
project(CookieClicker)

set(SDL3_INCLUDE_DIR C:/Users/marce/cpp_libraries/SDL3-3.2.0/include)
set(SDL3_LIBRARY C:/Users/marce/cpp_libraries/libs/SDL3.dll)
set(CMAKE_CXX_FLAGS "-mwindows")
set(CMAKE_C_STANDARD 23)


include_directories(${SDL3_INCLUDE_DIR})
add_executable(CookieClicker CookieClicker.c CookieClicker.h)
target_link_libraries(CookieClicker ${SDL3_LIBRARY})