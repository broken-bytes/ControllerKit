#include "GamingInputController.hxx"

#include "Controller.hxx"
#include "Types.hxx"

namespace BrokenBytes::ControllerKit::Internal {
	GamingInputController::GamingInputController(
		const Input::Gamepad* gamepad
	) : Controller(Types::ControllerType::XBoxOne), IRumbleController(), IImpulseTriggerController() {
		_gamepad = gamepad;
	}

	bool GamingInputController::operator==(const Input::Gamepad* gamepad) const {
		return gamepad == _gamepad;
	}

	bool GamingInputController::operator==(const Input::Gamepad& rhs) const {
		return &rhs == _gamepad;
	}

	auto GamingInputController::Equals(void* data) -> bool {
		return this->_gamepad == data;
	}

	void GamingInputController::SetRumble(Rumble motor, uint8_t strength) {
		
	}

	auto GamingInputController::Gamepad() const -> const winrt::Windows::Gaming::Input::Gamepad* {
		return _gamepad;
	}
}
