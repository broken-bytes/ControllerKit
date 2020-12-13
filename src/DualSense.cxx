#include "DualSense.hxx"
#include "Mapper.hxx"
#include "ControllerKit.hxx"

using namespace BrokenBytes::ControllerKit::Math;
using namespace BrokenBytes::ControllerKit::Types;

namespace BrokenBytes::ControllerKit::Internal {
	DualSense::DualSense(DevicePath path) :
		HIDController(path, ControllerType::DualSense),
		IRumbleController(),
		IGyroscopeController(),
		ILightbarController(),
		IAdaptiveTriggerController(),
		ITouchpadController() {
		_report = new unsigned char[DUALSENSE_WRITE_REPORT_SIZE];
		_thread = std::thread([this]() {this->Routine(); });
	}

	DualSense::~DualSense() {
		HIDController::~HIDController();
	}

	auto DualSense::SetPlayerIndicator(uint8_t player) -> void {
		SetPermission(Permission1::None, Permission2::TogglePlayerLED);
		_report[44] = player;
		SetDirty();
	}

	auto DualSense::ReadGyroscope() -> Math::Vector3<float> {
		return { 0,0,0 };
	}

	auto DualSense::ReadAcceleration() -> Math::Vector3<float> {
		return { 0,0,0 };
	}

	auto DualSense::SetLightbarColor(Color c) -> void {
		_report[45] = c.R;
		_report[46] = c.G;
		_report[47] = c.B;
		SetPermission(Permission1::None, Permission2::Lightbar);
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
		const auto trPerm = (trigger == Trigger::Left) ?
			Permission1::LeftTrigger :
			Permission1::RightTrigger;
		SetPermission(trPerm, Permission2::MotorPower);
		if (trigger == Trigger::Right) {
			_report[11] = static_cast<uint8_t>(mode);
			_report[12] = params.Start;
			_report[13] = params.ForceOrEnd;
			_report[14] = params.ForceInRange;
			_report[15] = params.Strength.Released;
			_report[16] = params.Strength.Middle;
			_report[17] = params.Strength.Pressed;
			_report[20] = params.Frequency;
		}
		else {
			_report[22] = static_cast<uint8_t>(mode);
			_report[23] = params.Start;
			_report[24] = params.ForceOrEnd;
			_report[25] = params.ForceInRange;
			_report[26] = params.Strength.Released;
			_report[27] = params.Strength.Middle;
			_report[28] = params.Strength.Pressed;
			_report[31] = params.Frequency;
		}

		SetDirty();
	}

	auto DualSense::GetTouches() -> std::vector<Math::Vector2<float>> {
		return std::vector<Vector2<float>>(0, { 0,0 });
	}

	auto DualSense::SetDirty() -> void {
		HIDController::SetDirty();
	}
	auto DualSense::SetClear() -> void {
		memset(_report, 0, DUALSENSE_WRITE_REPORT_SIZE);
		_report[0] = 0x02;
		// Make motors fade out and enable audio haptics
		_report[1] = 0x01;
		_report[2] = 0x00;
		HIDController::SetClear();
	}

	auto DualSense::SetPermission(Permission1 perm1, Permission2 perm2) const -> void {
		_report[1] |= static_cast<uint8_t>(perm1);
		_report[2] |= static_cast<uint8_t>(perm2);
	}

	auto DualSense::Routine() -> void {
		auto* buffer = new unsigned char[DUALSENSE_READ_REPORT_SIZE];
		SetClear();

		while (Device() != nullptr) {
			memset(buffer, 0, DUALSENSE_READ_REPORT_SIZE);
			size_t bytesRead = DUALSENSE_READ_REPORT_SIZE;
			ReadReport(buffer, bytesRead);
			SetInputReport(Mapping::InputReportFromDualSense(buffer));
			if (IsDirty()) {
				size_t write = static_cast<uint8_t>(DUALSENSE_WRITE_REPORT_SIZE);
				SendReport(_report, write);
				SetClear();
			}
		}
		delete[] buffer;
	}
}
