#include "DualSense.hxx"
#include "Mapper.hxx"

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
		_thread = std::thread([this]() {this->Routine(); });
	}

	DualSense::~DualSense() {
		HIDController::~HIDController();
	}

	bool DualSense::operator==(char* path) const {
		return true;
	}
	
	bool DualSense::operator==(const char* path) const {
		return true;
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
		while (true) {
			size_t bytesRead = 0;
			ReadReport(buffer, bytesRead);
			ParseRawInput(buffer);
			if(bytesRead > 0) {
				std::cout << ">" << std::endl;
			}
			if (_isDirty) {
				size_t read = DUALSENSE_WRITE_REPORT_SIZE;
				SendReport(_report, read);
				SetClear();
			}
		}
		delete buffer;
	}

	auto DualSense::ParseRawInput(unsigned char* input) -> void {
		SetInputReport(Mapping::InputReportFromDualSense(input));
	}
}
