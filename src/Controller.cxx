#include "Controller.hxx"
#include "ControllerKit.hxx"


namespace BrokenBytes::ControllerKit::Internal {
	Controller::Controller(ControllerType type) {
		_lastReport = {};
		_report = {};
		this->_type = type;
	}

	Vector2<float> Controller::GetStick(uint8_t id) const {
		if (id == 0) {
			return _report.LeftStick;
		}
		return _report.RightStick;
	}

	float Controller::GetTrigger(Trigger t) const {
		if (t == Trigger::Left) {
			return _report.LeftTrigger;
		}
		return _report.RightTrigger;
	}

	DPadDirection Controller::GetDPadDirection() const {
		return _report.DPad;
	}

	ButtonState Controller::GetButtonState(Button button) const {
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

	ControllerType Controller::Type() const {
		return _type;
	}

	void Controller::SetInputReport(InputReport report) {
		_lastReport = report;
		_report = report;
	}
}
