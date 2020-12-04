#include <thread>
#include <chrono>
#include <iostream>

#include "DualShock4.hxx"
#include "ControllerKit.hxx"

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

	auto DualShock4::Create(char* path) -> DualShock4* {
		for (auto item : Controller::controllers) {
			if (typeid(item.second) == typeid(DualShock4)) {
				auto* ds = dynamic_cast<DualShock4*>(item.second);
				if(ds->Path() == path) {
					return ds;
				}
			}
		}
		return new DualShock4(path);
	}
	auto DualShock4::Remove(char* path) -> void {
		for (auto item : Controller::controllers) {
			if (typeid(item.second) == typeid(DualShock4)) {
				auto* ds = dynamic_cast<DualShock4*>(item.second);
				if (ds->Path() == path) {
					delete ds;
					break;
				}
			}
		}
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

		_leftStick = Vector2<float>{
			ConvertToSignedFloat(buf[1]),
			ConvertToSignedFloat(buf[2])
		};

		_rightStick = Vector2<float>{
			ConvertToSignedFloat(buf[3]),
			ConvertToSignedFloat(buf[4])
		};

		InputReport report;

		auto buttons = std::map<Button, bool>();
		std::bitset<8> buff5(buf[5]);
		buttons.emplace(Button::Square, buff5[4] );
		buttons.emplace(Button::Cross, buff5[5]);
		buttons.emplace(Button::Circle, buff5[6]);
		buttons.emplace(Button::Triangle, buff5[7]);

		std::bitset<8> buff6(buf[6]);
		buttons.emplace(Button::L1, buff6[0]);
		buttons.emplace(Button::R1, buff6[1]);
		buttons.emplace(Button::R2, buff6[2]);
		buttons.emplace(Button::L2, buff6[3]);
		buttons.emplace(Button::Share, buff6[4]);
		buttons.emplace(Button::Options, buff6[5]);
		buttons.emplace(Button::L3, buff6[6]);
		buttons.emplace(Button::R3, buff6[7]);

		std::bitset<8> buff7(buf[6]);
		buttons.emplace(Button::PS, buff6[0]);
		buttons.emplace(Button::TouchPad, buff6[1]);

		_dPad = _dPadDirections[buf[5] & 0b00001111];

		report.Buttons = buttons;
		report.LeftTrigger = static_cast<float>(buf[8] / 255.0f);
		report.RightTrigger = buf[9] / 255.0f;
		
		SetInputReport(report);
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

		SendReport(report, sizeof(report));
	}
}
