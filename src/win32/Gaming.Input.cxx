#include <wrl.h>
#include <roapi.h>

#include "Gaming.Input.hxx"

namespace BrokenBytes::ControllerKit::GamingInput {
	auto Init() -> void {
		WINRT_RoInitialize(RO_INIT_MULTITHREADED);
	}

	auto Gamepads() -> Foundation::IVectorView<Gaming::Gamepad> {
		return Gaming::Gamepad::Gamepads();
	}
}
