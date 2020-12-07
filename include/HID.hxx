#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>
#include <vector>

#ifdef _WIN32
typedef HANDLE HIDDevice;
typedef wchar_t* DevicePath;
#endif
typedef unsigned char byte;

namespace BrokenBytes::ControllerKit::HID {	
	auto OpenDevice(DevicePath path, HIDDevice* devicePtr) -> void;
	auto CloseDevice(HIDDevice devicePtr) -> void;
	auto WriteToDevice(HIDDevice device, byte* data, size_t length) -> size_t;
	auto ReadFromDevice(HIDDevice device, byte* data, size_t length) -> size_t;
	auto SetOutputReport(HIDDevice device, byte* data, size_t length) -> size_t;	

}
