#include "Gaming.Input.hxx"

namespace BrokenBytes::ControllerKit::GamingInput {
	auto Init() -> void {
		auto devices = Gaming::Gamepad::Gamepads();
	}

	auto Gamepads() -> Foundation::IVectorView<Gaming::Gamepad> {
		return Gaming::Gamepad::Gamepads();
	}
}
