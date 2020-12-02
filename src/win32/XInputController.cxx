#include "XInputController.hxx"

namespace BrokenBytes::ControllerKit::Internal {

	XInputController::XInputController(XInputDevice gamepad) :
		IRumbleController() {
		_gamepad = std::move(gamepad);
	}

	auto XInputController::SetRumble(Rumble motor, uint8_t strength) -> void {}
}
