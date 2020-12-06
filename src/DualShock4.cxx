#include <thread>
#include <chrono>
#include <iostream>

#include "DualShock4.hxx"
#include "ControllerKit.hxx"
#include "Mapper.hxx"

using namespace BrokenBytes::ControllerKit::Math;
using namespace BrokenBytes::ControllerKit::Types;

namespace BrokenBytes::ControllerKit::Internal {
	DualShock4::DualShock4(char* path) :
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

	void DualShock4::Routine() {
		unsigned char buf[256];
		memset(buf, 0, sizeof(buf));
		buf[0] = 0x01;

		memset(buf, 0, 256);
		while (Device() != nullptr) {
			ReadReports();
			WriteReports();
			std::this_thread::sleep_for(
				std::chrono::milliseconds(_pollRateMs)
			);
		}
	}

	void DualShock4::ReadReports() {
		unsigned char buf[256];
		memset(buf, 0, sizeof(buf));
		buf[0] = 0x02;
		buf[1] = 0x01;
		SetInputReport(Mapping::InputReportFromDualShock4(buf));
	}

	Math::Vector3<float> DualShock4::ReadGyroscope() {
		return { 0,0, 0 };
	}
	Math::Vector3<float> DualShock4::ReadAcceleration() {
		return { 0,0,0 };
	}
	void DualShock4::SetLightbarColor(Color c) {}
	void DualShock4::SetRumble(Rumble motor, uint8_t strength) {}
	std::vector<Math::Vector2<uint8_t>> DualShock4::GetTouches() {
		return { { 0,0 } };
	}

	void DualShock4::WriteReports() {
		unsigned char report[32];
		memset(report, 0, 32);
		// Config
		report[0] = 0x05;
		report[1] = 0xF7;

		// Controls
		report[2] = 0x04;
		// Rumble
		report[4] = _rumble[0];
		report[5] = _rumble[1];

		// LED

		report[6] = _ledColor.R;
		report[7] = _ledColor.G;
		report[8] = _ledColor.B;

		// Flash
		report[9] = _flashing[0];
		report[10] = _flashing[1];

		// Volume
		report[19] = 0x00;
		report[20] = 0x00;
		report[21] = 0x00;

		auto crc = GetCRCFromBytes(report, 28);
		report[28] = crc[0];
		report[29] = crc[1];
		report[30] = crc[2];
		report[31] = crc[3];

		size_t read = DUALSHOCK4_WRITE_REPORT_SIZE;
		SendReport(report, read);
		delete report;
	}
}
