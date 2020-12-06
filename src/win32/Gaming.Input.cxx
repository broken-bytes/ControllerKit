#include "Gaming.Input.hxx"

namespace BrokenBytes::ControllerKit::GamingInput {
	auto Init() -> void {
	}

	auto Gamepads() -> Foundation::IVectorView<Gaming::Gamepad> {
		return Gaming::Gamepad::Gamepads();
	}
}
