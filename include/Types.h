#pragma once
#include <cstdint>


struct ControllerKitColor {
	uint8_t R;
	uint8_t G;
	uint8_t B;
};

struct ControllerKitStick {
	float X;
	float Y;
};

struct ControllerKitTouch {
	float X;
	float Y;
	uint8_t Index;
};

struct ControllerKitGyroscope {
	float X;
	float Y;
	float Z;
};

enum ControllerKitFeature {
	Rumble,
	Gyroscope,
	Lightbar,
	AdaptiveTriggers,
	ImpulseTriggers,
	Touchpad
};

enum ControllerKitButtonState {
	ButtonDown,
	ButtonPressed,
	ButtonUp,
	ButtonReleased
};

enum ControllerKitDPadDirection {
	DPadNone,
	DPadLeft = 1,
	DPadUp = 2,
	DPadLeftUp = 3,
	DPadRight = 4,
	DPadRightUp = 6,
	DPadDown = 8,
	DPadLeftDown = 9,
	DPadRightDown = 12
};

enum ControllerKitTrigger {
	TriggerLeft,
	TriggerRight
};

enum  ControllerKitRumble {
	RumbleLeft,
	RumbleRight,
	RumbleTriggerLeft,
	RumbleTriggerRight
};

enum  ControllerKitControllerType {
	ControllerXBoxOne,
	ControllerXBoxSeries,
	ControllerDualShock4,
	ControllerDualSense,
	ControllerGeneric
};

enum ControllerKitAxis {
	LeftX,
	LeftY,
	RightX,
	RightY,
	LeftTrigger,
	RightTrigger
};

enum ControllerKitButton {
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

enum  ControllerKitAdaptiveTriggerMode {
	Disabled = 0x00,
	Continuous = 0x01,
	Sectional = 0x02,
	Advanced = 0x26
};