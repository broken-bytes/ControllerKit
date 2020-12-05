#pragma once

#include <vector>

#include "Controller.hxx"
#include "Types.hxx"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#ifdef _LIB_DYNAMIC
#define DLL_EXPORT __declspec( dllexport )
#else
#define DLL_EXPORT 
#endif
#endif

namespace BrokenBytes::ControllerKit {
	DLL_EXPORT void Init();
	DLL_EXPORT auto Controllers()->std::vector<Types::Controller>;
	DLL_EXPORT auto GetControllerType(int controller)-> Types::ControllerType;
	DLL_EXPORT auto OnControllerConnected(
		std::function<void(uint8_t id, Types::ControllerType type)> controller
	) -> void;
	DLL_EXPORT auto OnControllerDisconnected(
		std::function<void(uint8_t id)> controller
	) -> void;
#ifdef _WIN32
#ifdef _LIB_DYNAMIC
	BOOL WINAPI DllMain(
		_In_ HINSTANCE hinstDLL,
		_In_ DWORD     fdwReason,
		_In_ LPVOID    lpvReserved
	);
#endif
#endif
}
