#include <wrl.h>
#include <roapi.h>

#include "Gaming.Input.hxx"
#include <Windows.h>

namespace BrokenBytes::ControllerKit::GamingInput {
	auto Init() -> void {
		RoInitialize(RO_INIT_MULTITHREADED);
	}

	auto Gamepads() -> Foundation::IVectorView<Gaming::Gamepad> {
		return Gaming::Gamepad::Gamepads();
	}
}
