#pragma once

#include "Interface.hxx"

namespace BrokenBytes::ControllerKit::Types {
	struct Color {
		uint8_t R;
		uint8_t G;
		uint8_t B;
	};

	enum class Feature {
		Rumble,
		Gyroscope,
		Lightbar,
		AdaptiveTriggers,
		ImpulseTriggers,
		Touchpad
	};

	enum class ButtonState {
		Down,
		Pressed,
		Up,
		Released
	};

	enum class DPadDirection {
		None,
		Left,
		LeftUp,
		Up,
		RightUp,
		Right,
		RightDown,
		Down,
		LeftDown
	};

	enum class Trigger {
		Left,
		Right
	};

	enum class Rumble {
		Left,
		Right,
		TriggerLeft,
		TriggerRight
	};

	enum class ControllerType {
		XBoxOne,
		XBoxSeries,
		DualShock4,
		DualSense,
		Generic
	};

	enum class Axis {
		LeftX,
		LeftY,
		RightX,
		RightY,
		LeftTrigger,
		RightTrigger
	};

	enum class Button {
		Button0, Cross = 0, A = 0,
		Button1, Square = 1, X = 1,
		Button2, Circle = 2, B = 2,
		Button3, Triangle = 3, Y = 3,
		Button4, L1 = 4, LB = 4,
		Button5, R1 = 5, RB = 5,
		Button6, L2 = 6, LT = 6,
		Button7, R2 = 7, RT = 7,
		Button8, Share = 8, Create = 8, View = 8,
		Button9, Options = 9, Menu = 9,
		Button10, L3 = 10,
		Button11, R3 = 11,
		Button12, PS = 12, XBox = 12,
		Button13, TouchPad = 13, XBox_Share = 13,
		Button14, PS_MUTE = 14,
		Button15 = 15,
		Button16 = 16
	};

	enum class AdaptiveTriggerMode {
		Disabled = 0x00,
		Continuous = 0x01,
		Sectional = 0x02,
		Advanced = 0x26
	};
}