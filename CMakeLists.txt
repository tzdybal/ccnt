cmake_minimum_required(VERSION 2.8.11)

project(ccnt)

find_package(OpenCV REQUIRED)

add_executable(ccnt main.cpp
	#window.h window.cpp
	#counter.h counter.cpp
)
set_property(TARGET ccnt PROPERTY CXX_STANDARD 11)

target_link_libraries(ccnt ${OpenCV_LIBS})
