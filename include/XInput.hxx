#pragma once
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <concrt.h>
#include <winrt/Windows.Gaming.Input.h>
#include <Xinput.h>

typedef XINPUT_GAMEPAD XInputDevice;
#endif


namespace BrokenBytes::ControllerKit::XInput {
	auto GetReading() -> const XINPUT_STATE;
	auto SetVibration(uint8_t motorId, uint8_t strength) -> void;
}
