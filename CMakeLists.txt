cmake_minimum_required(VERSION 3.21)
project(PParser)

set(CMAKE_CXX_STANDARD 23)

add_executable(PParser main.cpp components/person.cpp components/person.h components/manufacturer.cpp components/manufacturer.h components/building.cpp components/building.h components/workshop.cpp components/workshop.h components/project.cpp components/project.h dataManager.cpp dataManager.h utils.cpp utils.h configPaths.h)

target_link_libraries(PParser libtinyLogger.so)
target_link_libraries(PParser libtinyConfig.so)
target_link_libraries(PParser mysqlcppconn)

