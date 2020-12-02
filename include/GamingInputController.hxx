#pragma once

#include "XInput.hxx"
#include "interfaces/IRumbleController.hxx"
#include "interfaces/IImpulseTriggerController.hxx"


namespace BrokenBytes::ControllerKit::Internal {
	class XInputController : IRumbleController, IImpulseTriggerController {
	public:
		XInputController(XInputDevice gamepad);
		void SetRumble(Rumble motor, uint8_t strength) override;
		void SetVibration(Rumble r) override;
	private:
		XInputDevice _gamepad;
	};
}
