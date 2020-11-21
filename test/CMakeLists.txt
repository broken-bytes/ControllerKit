cmake_minimum_required(VERSION 3.14)

project(ControllerKitTest)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VS_STARTUP_PROJECT ControllerKit)



set(ControllerKit 
	../libs/ControllerKit/
)


set(src
 src/main.cxx
)


add_executable(Tests ${src})
target_link_libraries(Tests ${ControllerKit}ControllerKit)
target_include_directories(Tests PUBLIC libs/)