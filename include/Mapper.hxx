#pragma once

#include <bitset>
#include <map>
#include <memory>
#include <winrt/Windows.Gaming.Input.h>
#include "ControllerKit.hxx"

using namespace winrt::Windows::Gaming::Input;

namespace BrokenBytes::ControllerKit::Mapping {
	/// <summary>
	/// Holds the raw bianry values for each DPad state on DualSense and Dualshock(3?)4 devices
	/// </summary>
	enum class DS_DPad_Bin {
		Up = 0x00,
		RightUp = 0x01,
		Right = 0x02,
		RightDown = 0x03,
		Down = 0x04,
		LeftDown = 0x05,
		Left = 0x06,
		LeftUp = 0x07,
		None = 0x08
	};

	inline auto InputReportFromXBoxOne(const GamepadReading& input)->Internal::InputReport {
		auto buttons = std::map<uint8_t, uint8_t>();
		buttons.emplace(
			static_cast<uint8_t>(A),
			static_cast<uint8_t>(input.Buttons & GamepadButtons::A)
		);
		buttons.emplace(
			static_cast<uint8_t>(B),
			static_cast<uint8_t>(input.Buttons & GamepadButtons::B)
		);
		buttons.emplace(
			static_cast<uint8_t>(X),
			static_cast<uint8_t>(input.Buttons & GamepadButtons::X)
		);
		buttons.emplace(
			static_cast<uint8_t>(Y),
			static_cast<uint8_t>(input.Buttons & GamepadButtons::Y)
		);
		buttons.emplace(
			static_cast<uint8_t>(LB),
			static_cast<uint8_t>(input.Buttons & GamepadButtons::LeftShoulder)
		);
		buttons.emplace(
			static_cast<uint8_t>(RB),
			static_cast<uint8_t>(input.Buttons & GamepadButtons::RightShoulder)
		);
		buttons.emplace(
			static_cast<uint8_t>(View),
			static_cast<uint8_t>(input.Buttons & GamepadButtons::View)
		);
		buttons.emplace(
			static_cast<uint8_t>(Menu),
			static_cast<uint8_t>(input.Buttons & GamepadButtons::Menu)
		);
		buttons.emplace(
			static_cast<uint8_t>(L3),
			static_cast<uint8_t>(input.Buttons & GamepadButtons::LeftThumbstick)
		);
		buttons.emplace(
			static_cast<uint8_t>(R3),
			static_cast<uint8_t>(input.Buttons & GamepadButtons::RightThumbstick)
		);

		uint8_t dpad = 0;
		dpad += static_cast<uint8_t>(input.Buttons & GamepadButtons::DPadLeft) * 1;
		dpad += static_cast<uint8_t>(input.Buttons & GamepadButtons::DPadUp) * 2;
		dpad += static_cast<uint8_t>(input.Buttons & GamepadButtons::DPadRight) * 4;
		dpad += static_cast<uint8_t>(input.Buttons & GamepadButtons::DPadDown) * 8;

		Internal::InputReport rep {
			buttons, 
			{
				Math::ConvertToUnsignedShort(input.LeftThumbstickX),
				Math::ConvertToUnsignedShort(input.LeftThumbstickY)
			},
			{
				Math::ConvertToUnsignedShort(input.RightThumbstickX),
				Math::ConvertToUnsignedShort(input.RightThumbstickY)
			},
			Math::ConvertToUnsignedShort(input.LeftTrigger),
			Math::ConvertToUnsignedShort(input.RightTrigger),
			dpad
		};
		return rep;
	}

	inline auto InputReportFromXBoxSeries()->Internal::InputReport {
		//return {  };
	}

	inline auto InputReportFromDualShock4(unsigned char* input)->Internal::InputReport {
		auto buttons = std::map<uint8_t, uint8_t>();
		std::bitset<8> buff5(input[5]);
		buttons.emplace(static_cast<uint8_t>(Cross), buff5[5]);
		buttons.emplace(static_cast<uint8_t>(Square), buff5[4]);
		buttons.emplace(static_cast<uint8_t>(Circle), buff5[6]);
		buttons.emplace(static_cast<uint8_t>(Triangle), buff5[7]);

		std::bitset<8> buff6(input[6]);
		buttons.emplace(static_cast<uint8_t>(L1), buff6[0]);
		buttons.emplace(static_cast<uint8_t>(R1), buff6[1]);
		buttons.emplace(static_cast<uint8_t>(R2), buff6[2]);
		buttons.emplace(static_cast<uint8_t>(L2), buff6[3]);
		buttons.emplace(static_cast<uint8_t>(Share), buff6[4]);
		buttons.emplace(static_cast<uint8_t>(Options), buff6[5]);
		buttons.emplace(static_cast<uint8_t>(L3), buff6[6]);
		buttons.emplace(static_cast<uint8_t>(R3), buff6[7]);

		std::bitset<8> buff7(input[6]);
		buttons.emplace(static_cast<uint8_t>(PS), buff6[0]);
		buttons.emplace(static_cast<uint8_t>(TouchPad), buff6[1]);

		ControllerKitDPadDirection dpad = DPadNone;
		switch (input[5] & 0x000F) {
		case static_cast<uint8_t>(DS_DPad_Bin::Up):
			dpad = DPadUp;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::RightUp) :
			dpad = DPadRightUp;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::Right) :
			dpad = DPadRight;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::RightDown) :
			dpad = DPadRightDown;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::Down) :
			dpad = DPadDown;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::LeftDown):
			dpad = DPadLeftDown;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::Left) :
			dpad = DPadLeft;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::LeftUp) :
			dpad = DPadLeftUp;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::None) :
			dpad = DPadNone;
			break;
		}

		Internal::InputReport report{
			buttons,
			{
				input[1],
				input[2]
			},
			{
				input[3],
				input[4]
			},
			input[8],
			input[9],
			static_cast<uint8_t>(dpad)
		};
		return report;
	}

	inline auto InputReportFromDualSense(unsigned char* input)->Internal::InputReport {
		auto buttons = std::map<uint8_t, uint8_t>();
		buttons.emplace(static_cast<uint8_t>(Cross), (bool)(input[8] & 0x20));
		buttons.emplace(static_cast<uint8_t>(Square), (bool)(input[8] & 0x10));
		buttons.emplace(static_cast<uint8_t>(Circle), (bool)(input[8] & 0x40));
		buttons.emplace(static_cast<uint8_t>(Triangle), (bool)(input[8] & 0x80));
		buttons.emplace(static_cast<uint8_t>(L1), (bool)(input[9] & 0x01));
		buttons.emplace(static_cast<uint8_t>(R1), (bool)(input[9] & 0x02));
		buttons.emplace(static_cast<uint8_t>(L2), (bool)(input[9] & 0x04));
		buttons.emplace(static_cast<uint8_t>(R2), (bool)(input[9] & 0x08));
		buttons.emplace(static_cast<uint8_t>(Create), (bool)(input[9] & 0x10));
		buttons.emplace(static_cast<uint8_t>(Options), (bool)(input[9] & 0x20));
		buttons.emplace(static_cast<uint8_t>(L3), (bool)(input[9] & 0x40));
		buttons.emplace(static_cast<uint8_t>(R3), (bool)(input[9] & 0x80));
		buttons.emplace(static_cast<uint8_t>(PS), (bool)(input[10] & 0x01));
		buttons.emplace(static_cast<uint8_t>(PS_MUTE), (bool)(input[10] & 0x04));
		buttons.emplace(static_cast<uint8_t>(TouchPad), (bool)(input[10] & 0x02));

		ControllerKitDPadDirection dpad = DPadNone;
		switch (input[8] & 0x000F) {
		case static_cast<uint8_t>(DS_DPad_Bin::Up) :
			dpad = DPadUp;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::RightUp) :
			dpad = DPadRightUp;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::Right) :
			dpad = DPadRight;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::RightDown) :
			dpad = DPadRightDown;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::Down) :
			dpad = DPadDown;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::LeftDown) :
			dpad = DPadLeftDown;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::Left) :
			dpad = DPadLeft;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::LeftUp) :
			dpad = DPadLeftUp;
			break;
		case static_cast<uint8_t>(DS_DPad_Bin::None) :
			dpad = DPadNone;
			break;
		}

		Internal::InputReport report{
			buttons,
			{
				input[1],
				input[2]
			},
			{
				input[3],
				input[4]
			},
			input[5],
			input[6],
			static_cast<uint8_t>(dpad)
		};
		return report;
	}
}
