#include "XInput.hxx"

namespace BrokenBytes::ControllerKit::XInput {
	auto GetGameControllers() -> std::vector<XInputDevice> {
		return std::vector<XInputDevice>();
	}

	auto GetReading() -> const XINPUT_STATE {
		return {};
	}
}
