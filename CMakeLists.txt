cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ControllerKit)
set(CMAKE_SUPPRESS_REGENERATION true)


set(CONTROLLERKIT ControllerKit)
set(WIN ${CONTROLLERKIT}_WIN)
set(WIN_10 ${CONTROLLERKIT}_WIN10)
set(LINUX ${CONTROLLERKIT}_LINUX)
set(OSX ${CONTROLLERKIT}_OSX)

set(DYNAMIC _DYNAMIC)
set(STATIC _STATIC)
set(MANAGED _MANAGED)

set(WRAPPER _WRAPPER)

set(CSHARP _CSHARP)

set(SRC_INCLUDE
	include/ControllerKit.hxx
	include/Interface.hxx
	include/USB.hxx
	include/HID.hxx
	include/Math.hxx
  	include/Controller.hxx
	include/HIDController.hxx
	include/DualShock4.hxx
	include/DualSense.hxx
	include/Mapper.hxx

)

set(SRC_INTERFACES
	include/interfaces/IRumbleController.hxx
	include/interfaces/ILightBarController.hxx
	include/interfaces/IGyroscopeController.hxx
	include/interfaces/ITouchpadController.hxx
	include/interfaces/IImpulseTriggerController.hxx
	include/interfaces/IAdaptiveTriggerController.hxx
)


set(SRC_INCLUDE_WIN
	include/XInputController.hxx
	include/GamingInputController.hxx
	include/XInput.hxx
	include/Gaming.Input.hxx
)

set(SRC
	src/ControllerKit.cxx
	src/Controller.cxx
	src/DualShock4.cxx
	src/DualSense.cxx
	src/HIDController.cxx
)

set(SRC_WIN
	src/win32/Interface.cxx
	src/win32/USB.cxx
	src/win32/HID.cxx
	src/win32/XInput.cxx
	src/win32/Gaming.Input.cxx
	src/win32/GamingInputController.cxx
	src/win32/XInputController.cxx
)

set(SRC_MANAGED
	plugins/managed/ControllerKit.hxx
	plugins/managed/ControllerKit.cxx
)

set(SRC_WRAPPER_CSHARP
	plugins/csharp/ControllerKit.cs
)


set(SRC_LIB ${SRC} ${SRC_INCLUDE} ${SRC_INTERFACES})

if(MSVC)
	add_library(${WIN_10}${STATIC} STATIC ${SRC_LIB})
	add_library(${WIN_10}${DYNAMIC} SHARED ${SRC_LIB})
	add_library(${WIN}${STATIC} STATIC ${SRC_LIB})
	add_library(${WIN}${DYNAMIC} SHARED ${SRC_LIB})
	add_library(${WIN_10}${MANAGED} SHARED ${SRC_MANAGED})
	set_target_properties(${WIN_10}${MANAGED} PROPERTIES LINKER_LANGUAGE CXX COMMON_LANGUAGE_RUNTIME "")
	add_library(${WIN_10}${WRAPPER}${CSHARP} SHARED ${SRC_WRAPPER_CSHARP})
	set_target_properties(${WIN_10}${WRAPPER}${CSHARP} PROPERTIES LINKER_LANGUAGE CSHARP)


	target_link_directories(${WIN_10}${STATIC} PUBLIC lib/)
	target_include_directories(${WIN_10}${STATIC} PUBLIC include/)
	target_include_directories(${WIN_10}${STATIC} PRIVATE external/)

	target_link_directories(${WIN_10}${DYNAMIC} PUBLIC lib/)
	target_include_directories(${WIN_10}${DYNAMIC} PUBLIC include/)
	target_include_directories(${WIN_10}${DYNAMIC} PRIVATE external/)

	target_link_directories(${WIN}${STATIC} PUBLIC lib/)
	target_include_directories(${WIN}${STATIC} PUBLIC include/)
	target_include_directories(${WIN}${STATIC} PRIVATE external/)

	target_link_directories(${WIN}${DYNAMIC} PUBLIC lib/)
	target_include_directories(${WIN}${DYNAMIC} PUBLIC include/)
	target_include_directories(${WIN}${DYNAMIC} PRIVATE external/)

	target_include_directories(${WIN_10}${MANAGED} PUBLIC include/)
	target_include_directories(${WIN_10}${MANAGED} PUBLIC include/)
	target_link_directories(${WIN_10}${MANAGED} PUBLIC lib/)

	add_executable(Test WIN32 EXCLUDE_FROM_ALL test/main.cxx)
	target_include_directories(Test PUBLIC include/)


	target_sources(${WIN_10}${STATIC} PRIVATE ${SRC_WIN} ${SRC_INCLUDE_WIN})
	target_sources(${WIN_10}${DYNAMIC} PRIVATE ${SRC_WIN} ${SRC_INCLUDE_WIN})
	target_sources(${WIN}${STATIC} PRIVATE ${SRC_WIN} ${SRC_INCLUDE_WIN})
	target_sources(${WIN}${DYNAMIC} PRIVATE ${SRC_WIN} ${SRC_INCLUDE_WIN})


	target_link_libraries(${WIN_10}${STATIC} PUBLIC hid Kernel32 WindowsApp runtimeobject)
	target_link_libraries(${WIN_10}${DYNAMIC} PUBLIC hid Kernel32 WindowsApp runtimeobject)
	target_link_libraries(${WIN}${STATIC} PUBLIC hid Kernel32 WindowsApp runtimeobject)
	target_link_libraries(${WIN}${DYNAMIC} PUBLIC hid Kernel32 WindowsApp runtimeobject)
	target_link_libraries(${WIN_10}${MANAGED} PUBLIC ${WIN_10}${STATIC})
	target_link_libraries(${WIN_10}${WRAPPER}${CSHARP} PUBLIC ${WIN_10}${MANAGED})

	target_compile_definitions(${WIN_10}${STATIC} PUBLIC -D _W10)
	target_compile_definitions(${WIN_10}${DYNAMIC} PUBLIC  -D _W10 _LYB_DYNAMIC)
	target_compile_definitions(${WIN_10}${MANAGED} PUBLIC -D _W10)
	target_compile_definitions(${WIN}${STATIC} PUBLIC -D)
	target_compile_definitions(${WIN}${DYNAMIC} PUBLIC -D _LYB_DYNAMIC)

	target_link_libraries(Test PUBLIC ${WIN_10}${STATIC})

endif()
