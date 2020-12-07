#include <thread>
#include <chrono>
#include <iostream>

#include "DualShock4.hxx"
#include "ControllerKit.hxx"
#include "Mapper.hxx"
#include "Types.hxx"

using namespace BrokenBytes::ControllerKit::Math;
using namespace BrokenBytes::ControllerKit::Types;

namespace BrokenBytes::ControllerKit::Internal {
	DualShock4::DualShock4(DevicePath path) :
		HIDController(path, ControllerType::DualShock4),
		IRumbleController(),
		IGyroscopeController(),
		ILightbarController(),
		ITouchpadController() {
		//_thread = std::thread(&DualShock4::Routine, this);
	}

	DualShock4::~DualShock4() {
		HIDController::~HIDController();
	}

	auto DualShock4::Routine() -> void {
		unsigned char buf[256];
		buf[0] = 0x02;
		buf[1] = 0x01;
		
		while (Device() != nullptr) {
			memset(buf, 0, sizeof(buf));
			size_t bytesRead = DUALSHOCK4_READ_REPORT_SIZE;
			ReadReport(buf, bytesRead);
			SetInputReport(Mapping::InputReportFromDualShock4(buf));
			if(IsDirty()) {
				size_t written = 0;
				auto crc = GetCRCFromBytes(
					_report,
					DUALSHOCK4_WRITE_REPORT_SIZE - 4
				);
				_report[28] = crc[0];
				_report[29] = crc[1];
				_report[30] = crc[2];
				_report[31] = crc[3];
				SendReport(_report, written);
			}
			std::this_thread::sleep_for(
				std::chrono::milliseconds(_pollRateMs)
			);
		}
	}

	auto DualShock4::SetDirty() -> void {
		_report[0] = 0x05;
		_report[1] = 0xF7;
		//Controls
		_report[2] = 0x04;

		// Volume
		_report[19] = 0xFF;
		_report[20] = 0xFF;
		_report[21] = 0xFF;

		HIDController::SetDirty();
	}

	auto DualShock4::SetClear() -> void {
		memset(_report, 0, DUALSHOCK4_WRITE_REPORT_SIZE);
		HIDController::SetClear();
	}

	auto DualShock4::ReadGyroscope() -> Math::Vector3<float> {
		return { 0,0, 0 };
	}

	auto DualShock4::ReadAcceleration() -> Math::Vector3<float> {
		return { 0,0,0 };
	}
	void DualShock4::SetLightbarColor(Color c) {
		_report[6] = c.R;
		_report[7] = c.G;
		_report[8] = c.B;
		SetDirty();
	}

	auto DualShock4::SetRumble(Rumble motor, uint8_t strength) -> void {
		if(motor == Rumble::Left) {
			_report[4] = strength;
		}
		if(motor == Rumble::Right) {
			_report[5] = strength;
		}
	}

	auto DualShock4::GetTouches() -> std::vector<Math::Vector2<float>> {
		return { { 0,0 } };
	}
}
