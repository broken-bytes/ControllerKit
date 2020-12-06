cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ControllerKit)


set(CMAKE_SUPPRESS_REGENERATION true)

set(src_include
	include/ControllerKit.hxx
	include/Interface.hxx
	include/USB.hxx
	include/HID.hxx
	include/Math.hxx
	include/Types.hxx
  include/Controller.hxx
	include/HIDController.hxx
	include/DualShock4.hxx
	include/DualSense.hxx
	include/Mapper.hxx

)

set(src_interfaces
	include/interfaces/IRumbleController.hxx
	include/interfaces/ILightBarController.hxx
	include/interfaces/IGyroscopeController.hxx
	include/interfaces/ITouchpadController.hxx
	include/interfaces/IImpulseTriggerController.hxx
	include/interfaces/IAdaptiveTriggerController.hxx
)


set(include_win
	include/XInputController.hxx
	include/GamingInputController.hxx
	include/XInput.hxx
	include/Gaming.Input.hxx
)

set(src
	src/ControllerKit.cxx
	src/Controller.cxx
	src/DualShock4.cxx
	src/DualSense.cxx
	src/HIDController.cxx
)

set(src_win
	src/win32/Interface.cxx
	src/win32/USB.cxx
	src/win32/HID.cxx
	src/win32/XInput.cxx
	src/win32/Gaming.Input.cxx
	src/win32/GamingInputController.cxx
	src/win32/XInputController.cxx
)


add_library(ControllerKit ${src} ${src_include} ${src_interfaces})
if(MSVC)
	add_executable(Test WIN32 test/main.cxx)
	target_sources(ControllerKit PRIVATE ${src_win} ${include_win})
	target_link_libraries(ControllerKit PUBLIC hid Kernel32 WindowsApp runtimeobject)
endif()
target_link_libraries(Test PUBLIC ControllerKit)
target_link_directories(ControllerKit PUBLIC lib/)
target_include_directories(ControllerKit PUBLIC include/)
target_include_directories(ControllerKit PRIVATE external/)
