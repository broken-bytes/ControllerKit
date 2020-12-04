#include "GamingInputController.hxx"

#include "Controller.hxx"
#include "Types.hxx"

namespace BrokenBytes::ControllerKit::Internal {
	GamingInputController::GamingInputController(
		const Input::Gamepad* gamepad
	) : Controller(Types::ControllerType::XBoxOne), IRumbleController(), IImpulseTriggerController() {
		_gamepad = gamepad;
	}

	auto GamingInputController::Create(
		const Input::Gamepad* gamepad
	) -> GamingInputController* {
		for(auto item: controllers) {
			if(typeid(item.second) == typeid(GamingInputController)) {
				auto* gc = dynamic_cast<GamingInputController*>(item.second);
				if(gc->Gamepad() == gamepad) {
					return gc;
				}
			}
		}
		return new GamingInputController(gamepad);
	}

	auto GamingInputController::Remove(const Input::Gamepad* gamepad) -> void {
		for (auto item : controllers) {
			if (typeid(item.second) == typeid(GamingInputController)) {
				auto* gc = dynamic_cast<GamingInputController*>(item.second);
				if (gc->Gamepad() == gamepad) {
					delete gc;
					break;
				}
			}
		}
	}

	void GamingInputController::SetRumble(Rumble motor, uint8_t strength) {
		
	}

	auto GamingInputController::Gamepad() const -> const winrt::Windows::Gaming::Input::Gamepad* {
		return _gamepad;
	}
}
