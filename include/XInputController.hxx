#pragma once

#include "XInput.hxx"
#include "interfaces/IRumbleController.hxx"


namespace BrokenBytes::ControllerKit::Internal {
	class XInputController : IRumbleController {
	public:
		XInputController(XInputDevice gamepad);
		auto SetRumble(Rumble motor, uint8_t strength) -> void override;
	private:
		XInputDevice _gamepad;
	};
}
