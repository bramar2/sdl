cmake_minimum_required(VERSION 3.27)
project(CookieClicker)

#set(SDL3_INCLUDE_DIR C:/Users/marce/cpp_libraries/SDL3-3.2.0/include)
#set(SDL3_LIBRARY C:/Users/marce/cpp_libraries/libs/SDL3.dll)
#set(CMAKE_C_FLAGS "-mwindows")
set(CMAKE_C_STANDARD 23)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
add_executable(CookieClicker CookieClicker.c CookieClicker.h CookieBmp.c CookieBmp.h Counter.c Counter.h Screen.h Utils.c Utils.h Cookie.c Cookie.h Cursor.c Cursor.h)
target_link_libraries(CookieClicker PRIVATE SDL3::SDL3 m)