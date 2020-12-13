#include <winrt/Windows.Foundation.Collections.h>
#include <concrt.h>

#include "Gaming.Input.hxx"
#include "GamingInputController.hxx"
#include "ControllerKit.hxx"
#include "Controller.hxx"
#include "Mapper.hxx"

concurrency::critical_section LOCK{};

using namespace BrokenBytes::ControllerKit::Types;
using namespace winrt::Windows::Gaming::Input;


namespace BrokenBytes::ControllerKit::Internal {
	GamingInputController::GamingInputController() : Controller(Types::ControllerType::XBoxOne), IRumbleController(), IImpulseTriggerController() {
		_vibration = { 0,0,0,0 };
		_worker = std::thread([this]() {this->Routine(); });
		if(_gamepads.empty()) {
			for(const auto& item: GamingInput::Gamepads()) {
				auto size = GamingInput::Gamepads().Size();
				_gamepads.emplace_back(item);
			}
			_gamepad = &_gamepads[0];
			auto test = _gamepad->GetCurrentReading().Buttons;
			return;
		}
		for(const auto& item: GamingInput::Gamepads()) {
			std::lock_guard g{ LOCK };
			auto it{
				std::find(std::begin(_gamepads),
				std::end(_gamepads),
				item)
			};

			if (it == std::end(_gamepads))
			{
				// This code assumes that you're interested in all gamepads.
				_gamepads.emplace_back(item);
				this->_gamepad = &item;
			}
		}
	}

	auto GamingInputController::Equals(void* data) -> bool {
		auto gamepad = static_cast<const Gaming::Gamepad*>(data);
		auto equal = *this->_gamepad == *gamepad;
		return equal;
	}

	void GamingInputController::SetRumble(Rumble motor, uint8_t strength) {
		if(_gamepad != nullptr) {
			auto current = _gamepad->Vibration();
			if (motor == Rumble::TriggerLeft) {
				current.LeftTrigger = Math::ConvertToUnsignedFloat(strength);
			}
			if (motor == Rumble::TriggerRight) {
				current.RightTrigger = Math::ConvertToUnsignedFloat(strength);
			}
			_gamepad->Vibration(current);
		}
	}

	auto GamingInputController::Routine() -> void {
		while (true) {
			if(_gamepad == nullptr) {
				continue;
			}
			SetInputReport(Mapping::InputReportFromXBoxOne(_gamepad->GetCurrentReading()));
		}
	}
}
