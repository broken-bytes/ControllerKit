cmake_minimum_required(VERSION 3.14)

project(ControllerKitLibrary)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VS_STARTUP_PROJECT ControllerKit)


set(libs 
    ${CMAKE_SOURCE_DIR}/libs/
)

set(api
    include/ControllerKit.hxx
)

set(headers
    include/Controller.hxx
    include/XBoxOne.hxx
    include/DualShock4.hxx
    include/Interface.hxx
    include/WorkerThread.hxx
)

source_group("API" FILES ${api})
source_group("Header Files" FILES ${headers})

set(src
	src/Controller.cxx
	src/XBoxOne.cxx
	src/DualShock4.cxx
    src/ControllerKit.cxx
)

if(WIN32)
set(src_platform
	src/win64/Interface.cxx
)
else()
set(src_platform)
endif()


add_library(ControllerKit STATIC ${src} ${src_platform} ${api} ${headers})
target_link_libraries(ControllerKit PRIVATE ${libs}hid.lib)
