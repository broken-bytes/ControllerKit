#include <utility>


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
		for(int x = 0; x < controllers.size(); x++) {
			if(controllers[x] == nullptr) {
				controllers[x] = controller;
				_OnConnected(x, controller->Type());
				return;
			}
		}
		controllers.emplace(controllers.size(), controller);
		if(_OnConnected == nullptr) {
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

	Vector2<float> Controller::GetStick(uint8_t id) const {
		std::scoped_lock<std::mutex> lock(_reportMtx);
		if (id == 0) {
			return _report.LeftStick;
		}
		return _report.RightStick;
	}

	auto Controller::GetTrigger(Trigger t) const -> float {
		std::scoped_lock<std::mutex> lock(_reportMtx);
		if (t == Trigger::Left) {
			return _report.LeftTrigger;
		}
		return _report.RightTrigger;
	}

	auto Controller::GetDPadDirection() const -> DPadDirection {
		std::scoped_lock<std::mutex> lock(_reportMtx);
		return _report.DPad;
	}

	auto Controller::GetButtonState(Button button) const -> ButtonState {
		if(_report.Buttons.empty()) {
			return ButtonState::Released;
		}
		if (_lastReport.Buttons.empty()) {
			return ButtonState::Released;
		}
		std::scoped_lock<std::mutex> lock(_reportMtx);
		uint8_t state = 0;
		state += _report.Buttons.at(static_cast<uint8_t>(button));
		state += _lastReport.Buttons.at(static_cast<uint8_t>(button)) * 2;

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
		std::scoped_lock<std::mutex> lock(_reportMtx);
		_lastReport = _report;
		_report = report;
	}
}
