#include "DualSense.hxx"

using namespace BrokenBytes::ControllerKit::Math;
using namespace BrokenBytes::ControllerKit::Types;

namespace BrokenBytes::ControllerKit::Internal {
	DualSense::DualSense(char* path) :
		HIDController(path, ControllerType::DualSense),
		IRumbleController(),
		IGyroscopeController(),
		ILightbarController(),
		IAdaptiveTriggerController(),
		ITouchpadController() {
		_report = new unsigned char[DUALSENSE_WRITE_REPORT_SIZE];
		SetClear();
		//_thread = std::thread(&DualSense::Routine, this);
	}

	DualSense::~DualSense() {
		HIDController::~HIDController();
	}

	auto DualSense::Create(char* path) -> DualSense* {
		for (auto item : controllers) {
			if (typeid(item.second) == typeid(DualSense)) {
				auto* ds = dynamic_cast<DualSense*>(item.second);
				if (ds->Path() == path) {
					return ds;
				}
			}
		}
		return new DualSense(path);
	}

	auto DualSense::Remove(char* path) -> void {
		for (auto item : controllers) {
			if (typeid(item.second) == typeid(DualSense)) {
				auto* ds = dynamic_cast<DualSense*>(item.second);
				if (ds->Path() == path) {
					delete ds;
					break;
				}
			}
		}
	}

	auto DualSense::ReadGyroscope() -> Math::Vector3<float> {
		return { 0,0,0 };
	}

	auto DualSense::ReadAcceleration() -> Math::Vector3<float> {
		return { 0,0,0 };
	}

	auto DualSense::SetLightbarColor(Color c) -> void {
		SetDirty();
	}

	auto DualSense::SetRumble(Rumble motor, uint8_t strength) -> void {
		SetDirty();
	}

	auto DualSense::SetTrigger(
		Trigger trigger,
		AdaptiveTriggerMode mode,
		Params params
	) -> void {
		_triggers[trigger] = TriggerConfig{ mode, params };
		const auto trPerm = (trigger == Trigger::Left) ?
			Permission1::LeftTrigger :
			Permission1::RightTrigger;
		SetPermission(trPerm, Permission2::MotorPower);
		SetDirty();
	}

	auto DualSense::GetTouches() -> std::vector<Math::Vector2<uint8_t>> {
		return std::vector<Vector2<uint8_t>> (0, {0,0});
	}

	auto DualSense::SetDirty() -> void {
		_isDirty = true;
	}

	auto DualSense::SetClear() -> void {
		_isDirty = false;
		// Make motors fade out and enable audio haptics
		_report[1] = 0x01;
		_report[2] = 0x00;
	}

	auto DualSense::SetPermission(Permission1 perm1, Permission2 perm2) const -> void {
		_report[1] |= static_cast<uint8_t>(perm1);
		_report[2] |= static_cast<uint8_t>(perm2);
	}


	auto DualSense::Routine() -> void {
		auto* buffer = new unsigned char[DUALSENSE_READ_REPORT_SIZE];
		size_t bytesRead = 0;
		ReadReport(buffer, bytesRead);
		if (bytesRead < DUALSENSE_READ_REPORT_SIZE) {
			throw std::exception("Missing some bytes");
		}
		if (_isDirty) {
			SendReport(_report, sizeof(_report));
			SetClear();
		}
	}

	auto DualSense::ParseRawInput(unsigned char* input) -> void {
		auto buttons = std::map<Button, bool>();
		buttons.emplace(Button::Square, input[8] & 0x10);
		buttons.emplace(Button::Cross, input[8] & 0x20);
		buttons.emplace(Button::Circle, input[8] & 0x40);
		buttons.emplace(Button::Triangle, input[8] & 0x80);
		buttons.emplace(Button::L1, input[9] & 0x01);
		buttons.emplace(Button::R1, input[9] & 0x02);
		buttons.emplace(Button::L2, input[9] & 0x04);
		buttons.emplace(Button::R2, input[9] & 0x08);
		buttons.emplace(Button::Create, input[9] & 0x10);
		buttons.emplace(Button::Options, input[9] & 0x20);
		buttons.emplace(Button::L3, input[9] & 0x40);
		buttons.emplace(Button::R3, input[9] & 0x80);
		buttons.emplace(Button::PS, input[10] & 0x01);
		buttons.emplace(Button::TouchPad, input[10] & 0x02);
		buttons.emplace(Button::PS_MUTE, input[10] & 0x04);
		buttons.emplace(Button::TouchPad, input[10] & 0x02);
		buttons.emplace(Button::TouchPad, input[10] & 0x02);

		DPadDirection dpad = DPadDirection::None;
		switch (input[8] & 0x000F) {
		case 0b0000:
			dpad = DPadDirection::Up;
			break;
		case 0b0001:
			dpad = DPadDirection::RightUp;
			break;
		case 0b0010:
			dpad = DPadDirection::Right;
			break;
		case 0b0011:
			dpad = DPadDirection::RightDown;
			break;
		case 0b0100:
			dpad = DPadDirection::Down;
			break;
		case 0b0101:
			dpad = DPadDirection::LeftDown;
			break;
		case 0b0110:
			dpad = DPadDirection::Left;
			break;
		case 0b0111:
			dpad = DPadDirection::LeftUp;
			break;
		case 0b1000:
			dpad = DPadDirection::None;
			break;
		}

		InputReport rep{
			buttons,
			{
				(static_cast<float>(input[1]) / 255.0f - 0.5f) * 2,
			(static_cast<float>(input[2]) / 255.0f - 0.5f) * 2,
			},
			{
				(static_cast<float>(input[3]) / 255.0f - 0.5f) * 2,
				(static_cast<float>(input[4]) / 255.0f - 0.5f) * 2,
			},
			static_cast<float>(input[5]) / 255.0f,
			static_cast<float>(input[6]) / 255.0f,
			dpad
		};

		SetInputReport(rep);
	}
}
