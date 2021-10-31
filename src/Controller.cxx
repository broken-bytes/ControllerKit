#include <utility>
#include "ControllerKit.hxx"
#include "Controller.hxx"

using namespace BrokenBytes::ControllerKit::Math;

namespace BrokenBytes::ControllerKit::Internal {
	Controller::Controller(ControllerKitControllerType type) {
		this->_type = type;
		this->_number = 0;
		for (int x = 0; x < controllers.size(); x++) {
			if (controllers[x] == nullptr) {
				controllers.emplace(x, this);
				if (_OnConnected == nullptr) {
					return;
				}
				_OnConnected(x, type);
			}
		}
	}

	Controller::~Controller() {
		if (_OnDisconnected == nullptr) {
			return;
		}
		_OnDisconnected(this->_number);
	}

	auto Controller::Controllers() -> std::map<uint8_t, Controller*> {
		return controllers;
	}

	auto Controller::Add(Controller* controller) -> void {
		for (int x = 0; x < controllers.size(); x++) {
			if (controllers[x] == nullptr) {
				controllers[x] = controller;
				_OnConnected(x, controller->Type());
				return;
			}
		}
		controllers.emplace(controllers.size(), controller);
		if (_OnConnected == nullptr) {
			return;
		}
		_OnConnected(
			static_cast<uint8_t>(controllers.size() - 1),
			controller->Type()
		);
	}

	auto Controller::OnControllerConnected(std::function<void(uint8_t id, ControllerKitControllerType type)> callback) -> void {
		_OnConnected = std::move(callback);
	}
	auto Controller::OnControllerDisconnected(std::function<void(uint8_t id)> callback) -> void {
		_OnDisconnected = std::move(callback);
	}

	auto Controller::Flush() -> void {
		_queue = {};
	}

	auto Controller::Next() -> void {
		if (_queue.size() < 2) {
			return;
		}
		_queue.pop();
	}

	auto Controller::GetStick(uint8_t id) const -> Vector2<float> {
		if (_queue.empty()) {
			return {0,0};
		}
		if (id == 0) {
			auto stick = _queue.front().LeftStick;
			return {
				Math::ConvertToSignedFloat(stick.X),
				Math::ConvertToSignedFloat(stick.Y)
			};
		}
		auto stick = _queue.front().RightStick;
		return {
			Math::ConvertToSignedFloat(stick.X),
			Math::ConvertToSignedFloat(stick.Y)
		};
	}

	auto Controller::GetTrigger(ControllerKitTrigger t) const -> float {
		if(_queue.empty()) {
			return 0;
		}
		if (t == TriggerLeft) {
			return _queue.front().LeftTrigger;
		}
		return _queue.front().RightTrigger;
	}

	auto Controller::GetDPadDirection() const -> ControllerKitDPadDirection {
		if (_queue.empty()) {
			return DPadNone;
		}
		ControllerKitDPadDirection dir = DPadNone;
		switch (_queue.front().DPad) {
		case 1:
			dir = DPadLeft;
			break;
		case 2:
			dir = DPadUp;
			break;
		case 3:
			dir = DPadLeftUp;
			break;
		case 4:
			dir = DPadRight;
			break;
		case 6:
			dir = DPadRightUp;
			break;
		case 8:
			dir = DPadDown;
			break;
		case 9:
			dir = DPadLeftDown;
			break;
		case 12:
			dir = DPadRightDown;
			break;
		default:
		case 0:
			dir = DPadNone;
			break;
		}
		return dir;
	}

	auto Controller::GetButtonState(ControllerKitButton button) const -> ControllerKitButtonState {
		if (_queue.empty()) {
			return ButtonReleased;
		}
		switch (_queue.front().Buttons.at(static_cast<uint8_t>(button))) {
		case 1: return ButtonDown;
		case 2: return ButtonUp;
		case 3: return ButtonPressed;
		case 0: 
		default: 
			return ButtonReleased;
		}
	}

	auto Controller::Type() const -> ControllerKitControllerType {
		return _type;
	}

	auto Controller::SetInputReport(InputReport report) -> void {
		if (_queue.empty()) {
			_queue.push(report);
			return;
		}
		
		auto changed = false;
		auto& lastReport = _queue.back();

		if(report.LeftTrigger != lastReport.LeftTrigger) {
			changed = true;
		}

		if (report.RightTrigger != lastReport.RightTrigger) {
			changed = true;
		}

		if(!Math::IsInLimits<uint8_t>(
				report.LeftStick,
				lastReport.LeftStick,
				1
			)) {
			changed = true;
		}

		if (report.DPad != lastReport.DPad) {
			changed = true;
		}
		
		for (auto& item : lastReport.Buttons) {
			switch (item.second) {
			case 1:
				if (report.Buttons[item.first] == 0) {
					report.Buttons[item.first] = 2;
					changed = true;
					break;
				}
				report.Buttons[item.first] = 3;
				changed = true;
				break;
			case 3:
				if (report.Buttons[item.first] == 0) {
					report.Buttons[item.first] = 2;
					changed = true;
					break;
				}
				report.Buttons[item.first] = 3;
				break;
			case 0:
				if(report.Buttons[item.first] == 1) {
					changed = true;
				}
				break;
			case 2:
				if (report.Buttons[item.first] == 1) {
					changed = true;
					report.Buttons[item.first] = 1;
					break;
				}
				changed = true;
				report.Buttons[item.first] = 0;
				break;
			default:
				break;
			}
		}
		if(!changed) {
			return;
		}
		_queue.push(report);
	}
}
