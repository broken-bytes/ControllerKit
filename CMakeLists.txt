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

set(src_managed
	plugins/managed/ControllerKit.hxx
	plugins/managed/ControllerKit.cxx
)

if(MSVC)
	add_library(ControllerKit_Win10_Static STATIC ${src} ${src_include} ${src_interfaces})
	add_library(ControllerKit_Win10_Dynamic SHARED ${src} ${src_include} ${src_interfaces})
	add_library(ControllerKit_Win_Static STATIC ${src} ${src_include} ${src_interfaces})
	add_library(ControllerKit_Win_Dynamic SHARED ${src} ${src_include} ${src_interfaces})
	add_library(ControllerKit_Win10_Managed SHARED ${src_managed})
	set_target_properties(ControllerKit_Win10_Managed PROPERTIES LINKER_LANGUAGE CXX COMMON_LANGUAGE_RUNTIME "")

	target_link_directories(ControllerKit_Win10_Static PUBLIC lib/)
	target_include_directories(ControllerKit_Win10_Static PUBLIC include/)
	target_include_directories(ControllerKit_Win10_Static PRIVATE external/)

	target_link_directories(ControllerKit_Win10_Dynamic PUBLIC lib/)
	target_include_directories(ControllerKit_Win10_Dynamic PUBLIC include/)
	target_include_directories(ControllerKit_Win10_Dynamic PRIVATE external/)

	target_link_directories(ControllerKit_Win_Static PUBLIC lib/)
	target_include_directories(ControllerKit_Win_Static PUBLIC include/)
	target_include_directories(ControllerKit_Win_Static PRIVATE external/)

	target_link_directories(ControllerKit_Win_Dynamic PUBLIC lib/)
	target_include_directories(ControllerKit_Win_Dynamic PUBLIC include/)
	target_include_directories(ControllerKit_Win_Dynamic PRIVATE external/)

	target_link_directories(ControllerKit_Win10_Managed PUBLIC lib/)

	add_executable(Test WIN32 test/main.cxx)

	target_sources(ControllerKit_Win10_Static PRIVATE ${src_win} ${include_win})
	target_sources(ControllerKit_Win10_Dynamic PRIVATE ${src_win} ${include_win})
	target_sources(ControllerKit_Win_Static PRIVATE ${src_win} ${include_win})

	target_link_libraries(ControllerKit_Win10_Static PUBLIC hid Kernel32 WindowsApp runtimeobject)
	target_link_libraries(ControllerKit_Win10_Dynamic PUBLIC hid Kernel32 WindowsApp runtimeobject)
	target_link_libraries(ControllerKit_Win_Static PUBLIC hid Kernel32 WindowsApp runtimeobject)
	target_link_libraries(ControllerKit_Win10_Managed PUBLIC hid Kernel32 WindowsApp runtimeobject ControllerKit_Win10_Static)


	target_compile_definitions(ControllerKit_Win10_Static PUBLIC -D _W10)
	target_compile_definitions(ControllerKit_Win10_Dynamic PUBLIC  -D _W10 _LYB_DYNAMIC)
	target_compile_definitions(ControllerKit_Win10_Managed PUBLIC -D _W10)
	target_compile_definitions(ControllerKit_Win_Static PUBLIC -D)
	target_compile_definitions(ControllerKit_Win_Dynamic PUBLIC -D _LYB_DYNAMIC)

	target_link_libraries(Test PUBLIC ControllerKit_Win10_Static)
elseif(UNIX)
	add_library(ControllerKit_Linux_Static STATIC ${src} ${src_include} ${src_interfaces})
	add_library(ControllerKit_Linux_Dynamic SHARED ${src} ${src_include} ${src_interfaces})
	target_link_directories(ControllerKit_Linux_Static PUBLIC lib/)
	target_include_directories(ControllerKit_Linux_Static PUBLIC include/)
	target_include_directories(ControllerKit_Linux_Static PRIVATE external/)
	target_link_directories(ControllerKit_Linux_Dynamic PUBLIC lib/)
	target_include_directories(ControllerKit_Linux_Dynamic PUBLIC include/)
	target_include_directories(ControllerKit_Linux_Dynamic PRIVATE external/)
else()
	add_library(ControllerKit_OSX_Static STATIC ${src} ${src_include} ${src_interfaces})
	add_library(ControllerKit_OSX_Dynamic SHARED ${src} ${src_include} ${src_interfaces})
	target_link_directories(ControllerKit_OSX_Static PUBLIC lib/)
	target_include_directories(ControllerKit_OSX_Static PUBLIC include/)
	target_include_directories(ControllerKit_OSX_Static PRIVATE external/)
	target_link_directories(ControllerKit_OSX_Dynamic PUBLIC lib/)
	target_include_directories(ControllerKit_OSX_Dynamic PUBLIC include/)
	target_include_directories(ControllerKit_OSX_Dynamic PRIVATE external/)
endif()
