#include <utility>


#include "Controller.hxx"
#include "Types.hxx"

using namespace BrokenBytes::ControllerKit::Math;
using namespace BrokenBytes::ControllerKit::Types;

namespace BrokenBytes::ControllerKit::Internal {
	Controller::Controller(ControllerType type) {
		_lastReport = {};
		_report = {};
		_queue.push({});
		this->_type = type;
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
		_OnConnected(controllers.size() - 1, controller->Type());
	}

	auto Controller::OnControllerConnected(std::function<void(uint8_t id, ControllerType type)> callback) -> void {
		_OnConnected = std::move(callback);
	}
	auto Controller::OnControllerDisconnected(std::function<void(uint8_t id)> callback) -> void {
		_OnDisconnected = std::move(callback);
	}

	auto Controller::Flush() -> void {
		_queue = {};
	}

	auto Controller::Next() -> void {
		while (_queue.size() < 2) {
			// Wait until we have two elements, so we can calculate the last state etc
		}
		_queue.pop();
		std::cout << +static_cast<uint8_t>(Type()) << "Next() Left: " << _queue.size() << std::endl;
	}

	Vector2<float> Controller::GetStick(uint8_t id) const {
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

	auto Controller::GetTrigger(Trigger t) const -> float {
		if (t == Trigger::Left) {
			return _queue.front().LeftTrigger;
		}
		return _queue.front().RightTrigger;
	}

	auto Controller::GetDPadDirection() const -> DPadDirection {
		DPadDirection dir = DPadDirection::None;
		switch (_queue.front().DPad) {
		case 1:
			dir = DPadDirection::Left;
			break;
		case 2:
			dir = DPadDirection::Up;
			break;
		case 3:
			dir = DPadDirection::LeftUp;
			break;
		case 4:
			dir = DPadDirection::Right;
			break;
		case 6:
			dir = DPadDirection::RightUp;
			break;
		case 8:
			dir = DPadDirection::Down;
			break;
		case 9:
			dir = DPadDirection::LeftDown;
			break;
		case 12:
			dir = DPadDirection::RightDown;
			break;
		default:
		case 0:
			dir = DPadDirection::None;
			break;
		}
		return dir;
	}

	auto Controller::GetButtonState(Button button) const -> ButtonState {
		switch (_queue.front().Buttons.at(static_cast<uint8_t>(button))) {
		case 1: return ButtonState::Down;
		case 2: return ButtonState::Up;
		case 3: return ButtonState::Pressed;
		case 0: 
		default: 
			return ButtonState::Released;
		}
	}

	auto Controller::Type() const -> ControllerType {
		return _type;
	}

	auto Controller::SetInputReport(InputReport report) -> void {
		_lastReport = _report;
		_report = report;

		if (_queue.size() < 2) {
			_queue.push(report);
			return;
		}
		for (auto& item : _queue.back().Buttons) {
			switch (item.second) {
			case 1:
			case 3:
				if (report.Buttons[item.first] == 0) {
					report.Buttons[item.second] = 2;
					break;
				}
				report.Buttons[item.second] = 3;
				break;
			case 0:
			default:
				break;
			}
		}
		_queue.push(report);
	}
}
