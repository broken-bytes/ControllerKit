#include "GamingInputController.hxx"

#include "Controller.hxx"
#include "Types.hxx"

namespace BrokenBytes::ControllerKit::Internal {
	GamingInputController::GamingInputController(
		const Input::Gamepad* gamepad
	) : Controller(Types::ControllerType::XBoxOne), IRumbleController(), IImpulseTriggerController() {
		_gamepad = gamepad;
	}

	bool GamingInputController::operator==(Input::Gamepad* gamepad) const {
		return gamepad == _gamepad;
	}

	void GamingInputController::SetRumble(Rumble motor, uint8_t strength) {
		
	}

	auto GamingInputController::Gamepad() const -> const winrt::Windows::Gaming::Input::Gamepad* {
		return _gamepad;
	}
}
