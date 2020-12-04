#include "Controller.hxx"
#include "Types.hxx"

using namespace BrokenBytes::ControllerKit::Math;
using namespace BrokenBytes::ControllerKit::Types;

namespace BrokenBytes::ControllerKit::Internal {
	Controller::Controller(ControllerType type) {
		_lastReport = {};
		_report = {};
		this->_type = type;
		for (int x = 0; x < controllers.size(); x++) {
			if(controllers[x] == nullptr) {
				controllers.emplace(x, this);
				_onConnected(x, type);
			}
		}
	}

	Controller::~Controller() {
		_onDisconnected(this->_number);
	}

	auto Controller::Controllers() -> std::map<uint8_t, Controller*> {
		return controllers;
	}

	auto Controller::OnControllerConnected(std::function<void(uint8_t id, ControllerType type)> callback) -> void {
		_onConnected = callback;
	}
	auto Controller::OnControllerDisconnected(std::function<void(uint8_t id)> callback) -> void {
		_onDisconnected = callback;
	}

	Vector2<float> Controller::GetStick(uint8_t id) const {
		if (id == 0) {
			return _report.LeftStick;
		}
		return _report.RightStick;
	}

	auto Controller::GetTrigger(Trigger t) const -> float {
		if (t == Trigger::Left) {
			return _report.LeftTrigger;
		}
		return _report.RightTrigger;
	}

	auto Controller::GetDPadDirection() const -> DPadDirection {
		return _report.DPad;
	}

	auto Controller::GetButtonState(Button button) const -> ButtonState {
		uint8_t state = 0;
		state += _report.Buttons.at(button);
		state += _report.Buttons.at(button) * 2;

		switch (state) {
		case 0: return ButtonState::Released;
		case 1: return ButtonState::Down;
		case 2: return ButtonState::Up;
		case 3:
		default: return ButtonState::Pressed;
		}
	}

	auto Controller::Type() const -> ControllerType {
		return _type;
	}

	auto Controller::SetInputReport(InputReport report) -> void {
		_lastReport = report;
		_report = report;
	}
}
