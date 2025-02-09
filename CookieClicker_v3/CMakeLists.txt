cmake_minimum_required(VERSION 3.27)
project(CookieClicker)

set(CMAKE_C_FLAGS "-mwindows")
set(CMAKE_C_STANDARD 23)

find_package(SDL3 REQUIRED)
add_executable(CookieClicker CookieClicker.c CookieClicker.h CookieBmp.c CookieBmp.h Counter.c Counter.h Screen.h Utils.c Utils.h Cookie.c Cookie.h Cursor.c Cursor.h)
target_link_libraries(CookieClicker SDL3::SDL3)
