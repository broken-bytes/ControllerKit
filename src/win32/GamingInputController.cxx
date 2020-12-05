#include <winrt/Windows.Foundation.Collections.h>

#include "Gaming.Input.hxx"
#include "GamingInputController.hxx"

#include "Controller.hxx"
#include "Types.hxx"

namespace BrokenBytes::ControllerKit::Internal {
	GamingInputController::GamingInputController() : Controller(Types::ControllerType::XBoxOne), IRumbleController(), IImpulseTriggerController() {
		if(_gamepads == nullptr) {
			_gamepads = GamingInput::Gamepads();
			//this->_addr = &_gamepads.GetAt(0);
			return;
		}
		auto updated = GamingInput::Gamepads();
		for(auto& item: updated) {
			bool add = true;
			if(_gamepads.Size() == 0) {
				this->_addr = &item;
				return;
			}
			for(auto& item2: _gamepads) {
				if(item == item2) {
					add = false;
					break;
				}
			}
			if(add) {
				this->_addr = &item;
			}
		}
	}

	auto GamingInputController::Equals(void* data) -> bool {
		return this->_addr == data;
	}

	void GamingInputController::SetRumble(Rumble motor, uint8_t strength) {

	}
}
