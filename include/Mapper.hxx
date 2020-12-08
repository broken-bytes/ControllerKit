#pragma once

#include <bitset>
#include <map>
#include <memory>
#include <winrt/Windows.Gaming.Input.h>
#include "Types.hxx"

using namespace BrokenBytes::ControllerKit::Types;
using namespace winrt::Windows::Gaming::Input;

namespace BrokenBytes::ControllerKit::Mapping {
	inline auto InputReportFromXBoxOne(const GamepadReading& input)->Internal::InputReport {
		Internal::InputReport rep{};
		rep.LeftTrigger = static_cast<float>(input.LeftTrigger);
		rep.RightTrigger = static_cast<float>(input.RightTrigger);
		rep.LeftStick = {
			static_cast<float>(input.LeftThumbstickX),
			static_cast<float>(input.LeftThumbstickY)
		};
		rep.RightStick = {
			static_cast<float>(input.RightThumbstickX),
			static_cast<float>(input.RightThumbstickY)
		};
		auto buttons = std::map<uint8_t, bool>();
		buttons.emplace(
			static_cast<uint8_t>(Button::A),
			static_cast<bool>(input.Buttons & GamepadButtons::A)
		);
		buttons.emplace(
			static_cast<uint8_t>(Button::B),
			static_cast<bool>(input.Buttons & GamepadButtons::B)
		);
		buttons.emplace(
			static_cast<uint8_t>(Button::X),
			static_cast<bool>(input.Buttons & GamepadButtons::X)
		);
		buttons.emplace(
			static_cast<uint8_t>(Button::Y),
			static_cast<bool>(input.Buttons & GamepadButtons::Y)
		);
		buttons.emplace(
			static_cast<uint8_t>(Button::LB),
			static_cast<bool>(input.Buttons & GamepadButtons::LeftShoulder)
		);
		buttons.emplace(
			static_cast<uint8_t>(Button::RB),
			static_cast<bool>(input.Buttons & GamepadButtons::RightShoulder)
		);
		buttons.emplace(
			static_cast<uint8_t>(Button::View),
			static_cast<bool>(input.Buttons & GamepadButtons::View)
		);
		buttons.emplace(
			static_cast<uint8_t>(Button::Menu),
			static_cast<bool>(input.Buttons & GamepadButtons::Menu)
		);
		buttons.emplace(
			static_cast<uint8_t>(Button::L3),
			static_cast<bool>(input.Buttons & GamepadButtons::LeftThumbstick)
		);
		buttons.emplace(
			static_cast<uint8_t>(Button::R3),
			static_cast<bool>(input.Buttons & GamepadButtons::RightThumbstick)
		);
		rep.Buttons = buttons;
		return rep;
	}

	inline auto InputReportFromXBoxSeries()->Internal::InputReport {
		//return {  };
	}

	inline auto InputReportFromDualShock4(unsigned char* input)->Internal::InputReport {
		auto buttons = std::map<uint8_t, bool>();
		std::bitset<8> buff5(input[5]);
		buttons.emplace(static_cast<uint8_t>(Button::Cross), buff5[5]);
		buttons.emplace(static_cast<uint8_t>(Button::Square), buff5[4]);
		buttons.emplace(static_cast<uint8_t>(Button::Circle), buff5[6]);
		buttons.emplace(static_cast<uint8_t>(Button::Triangle), buff5[7]);

		std::bitset<8> buff6(input[6]);
		buttons.emplace(static_cast<uint8_t>(Button::L1), buff6[0]);
		buttons.emplace(static_cast<uint8_t>(Button::R1), buff6[1]);
		buttons.emplace(static_cast<uint8_t>(Button::R2), buff6[2]);
		buttons.emplace(static_cast<uint8_t>(Button::L2), buff6[3]);
		buttons.emplace(static_cast<uint8_t>(Button::Share), buff6[4]);
		buttons.emplace(static_cast<uint8_t>(Button::Options), buff6[5]);
		buttons.emplace(static_cast<uint8_t>(Button::L3), buff6[6]);
		buttons.emplace(static_cast<uint8_t>(Button::R3), buff6[7]);

		std::bitset<8> buff7(input[6]);
		buttons.emplace(static_cast<uint8_t>(Button::PS), buff6[0]);
		buttons.emplace(static_cast<uint8_t>(Button::TouchPad), buff6[1]);

		DPadDirection dpad = DPadDirection::None;
		switch (input[5] & 0x000F) {
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

		Internal::InputReport report{
			buttons,
			Math::Vector2<float>{
			Math::ConvertToSignedFloat(input[1]),
			Math::ConvertToSignedFloat(input[2])
		},Math::Vector2<float> {
			Math::ConvertToSignedFloat(input[3]),
			Math::ConvertToSignedFloat(input[4])
		},Math::ConvertToUnsignedFloat(input[8]),
			Math::ConvertToUnsignedFloat(input[9]),
			dpad
		};
		return report;
	}

	inline auto InputReportFromDualSense(unsigned char* input)->Internal::InputReport {
		auto buttons = std::map<uint8_t, bool>();
		buttons.emplace(static_cast<uint8_t>(Button::Cross), input[8] & 0x20);
		buttons.emplace(static_cast<uint8_t>(Button::Square), input[8] & 0x10);
		buttons.emplace(static_cast<uint8_t>(Button::Circle), input[8] & 0x40);
		buttons.emplace(static_cast<uint8_t>(Button::Triangle), input[8] & 0x80);
		buttons.emplace(static_cast<uint8_t>(Button::L1), input[9] & 0x01);
		buttons.emplace(static_cast<uint8_t>(Button::R1), input[9] & 0x02);
		buttons.emplace(static_cast<uint8_t>(Button::L2), input[9] & 0x04);
		buttons.emplace(static_cast<uint8_t>(Button::R2), input[9] & 0x08);
		buttons.emplace(static_cast<uint8_t>(Button::Create), input[9] & 0x10);
		buttons.emplace(static_cast<uint8_t>(Button::Options), input[9] & 0x20);
		buttons.emplace(static_cast<uint8_t>(Button::L3), input[9] & 0x40);
		buttons.emplace(static_cast<uint8_t>(Button::R3), input[9] & 0x80);
		buttons.emplace(static_cast<uint8_t>(Button::PS), input[10] & 0x01);
		buttons.emplace(static_cast<uint8_t>(Button::PS_MUTE), input[10] & 0x04);
		buttons.emplace(static_cast<uint8_t>(Button::TouchPad), input[10] & 0x02);

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

		Internal::InputReport report{
			buttons,
			{
				Math::ConvertToSignedFloat(input[1]),
				Math::ConvertToSignedFloat(input[2])
			},
			{
				Math::ConvertToSignedFloat(input[3]),
				Math::ConvertToSignedFloat(input[4])
			},
			Math::ConvertToUnsignedFloat(input[5]),
			Math::ConvertToUnsignedFloat(input[6]),
			dpad
		};
		return report;
	}
}
