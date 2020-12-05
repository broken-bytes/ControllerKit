#include "GamingInputController.hxx"

#include "Controller.hxx"
#include "Types.hxx"

namespace BrokenBytes::ControllerKit::Internal {
	GamingInputController::GamingInputController(
		Input::Gamepad* addr
	) : Controller(Types::ControllerType::XBoxOne), IRumbleController(), IImpulseTriggerController() {
		_addr = addr;

		_t = std::thread([addr] {
			while (true) {
				auto pad = &addr;
				auto reading = addr->GetCurrentReading();
			}
			});
	}

	auto GamingInputController::Equals(void* data) -> bool {
		return this->_addr == data;
	}

	void GamingInputController::SetRumble(Rumble motor, uint8_t strength) {

	}
}
