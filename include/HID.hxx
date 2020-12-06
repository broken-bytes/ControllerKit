#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>
#include <vector>

namespace BrokenBytes::ControllerKit::HID {
#ifdef _WIN32
	typedef HANDLE HIDDevice;
#endif
	typedef uint8_t byte;
	
	auto OpenDevice(char* path, HIDDevice& devicePtr) -> void;
	auto CloseDevice(HIDDevice& devicePtr) -> void;
	auto WriteToDevice(HIDDevice device, byte* data, size_t length) -> size_t;
	auto ReadFromDevice(HIDDevice device, byte* data, size_t length) -> size_t;
	auto SetOutputReport(HIDDevice device, byte* data, size_t length) -> size_t;	

}
