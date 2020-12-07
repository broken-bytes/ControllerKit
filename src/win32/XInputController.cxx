#include "XInputController.hxx"

namespace BrokenBytes::ControllerKit::Internal {

	XInputController::XInputController(XInputDevice gamepad) :
		IRumbleController() {
		_gamepad = std::move(gamepad);
	}

	bool XInputController::operator==(XInputDevice device) const {
		return &device == &_gamepad;
	}

	auto XInputController::SetRumble(Types::Rumble motor, uint8_t strength) -> void {}
}
