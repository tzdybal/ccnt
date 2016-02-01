cmake_minimum_required(VERSION 2.8.11)

project(ccnt)

# Find the OpenCV library
find_package(OpenCV REQUIRED)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Tell CMake to create the helloworld executable
add_executable(ccnt main.cpp
	window.h window.cpp
	counter.h counter.cpp
	converter.h converter.cpp
)
set_property(TARGET ccnt PROPERTY CXX_STANDARD 11)


# Use the Widgets module from Qt 5.
target_link_libraries(ccnt Qt5::Widgets ${OpenCV_LIBS})
