#include "HID.hxx"

namespace BrokenBytes::ControllerKit::HID {
	auto OpenDevice(char* path, HIDDevice devicePtr) -> void {
		devicePtr = CreateFile(
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
			return 0;
		}
		return written;
	}

	auto ReadFromDevice(HIDDevice device, byte* data, size_t length) -> size_t {
		DWORD read = 0;
		if(!ReadFile(device, data, 255, &read, nullptr)) {
			return 0;
		}
		return read;
	}

	auto SetOutputReport(HIDDevice device, byte* data, size_t length) -> size_t {
		return 0;
	}
}
