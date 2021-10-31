#pragma once
#include <cstdint>

#include "XInput.hxx"
#include "interfaces/IRumbleController.hxx"
#include <Types.h>

namespace BrokenBytes::ControllerKit::Internal {
	class XInputController : IRumbleController {
	public:
		XInputController(XInputDevice gamepad);
		bool operator==(XInputDevice device) const;
		auto SetRumble(ControllerKitRumble motor, uint8_t strength) -> void override;
	private:
		XInputDevice _gamepad;
	};
}
