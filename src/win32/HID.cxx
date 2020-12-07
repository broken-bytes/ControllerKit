#include "HID.hxx"

#include <string>

namespace BrokenBytes::ControllerKit::HID {
	auto OpenDevice(DevicePath path, HIDDevice* devicePtr) -> void {
		*devicePtr = CreateFileW(
			path,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr,
			OPEN_EXISTING,
			0,
			nullptr
		);
	}

	auto CloseDevice(HIDDevice devicePtr) -> void {
		CloseHandle(devicePtr);
	}

	auto WriteToDevice(HIDDevice device, byte* data, size_t length) -> size_t {
		DWORD written = 0;
		if(!WriteFile(device, data, length, &written, nullptr)) {
			auto err = GetLastError();
			return 0;
		}
		return written;
	}

	auto ReadFromDevice(HIDDevice device, byte* data, size_t length) -> size_t {
		DWORD read = 0;
		memset(data, 0, length);
		if(!ReadFile(device, data, length, &read, nullptr)) {
			auto err = GetLastError();
			return 0;
		}
		return read;
	}

	auto SetOutputReport(HIDDevice device, byte* data, size_t length) -> size_t {
		return 0;
	}
}
