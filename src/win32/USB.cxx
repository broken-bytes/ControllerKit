#include <Windows.h>
#include <hidsdi.h>
#undef CALLBACK
#include <iostream>
#include <thread>

#include "USB.hxx"



namespace BrokenBytes::ControllerKit::USB {
	void Init() {
	}

	std::vector<USBDevice> EnumerateDevices() {
		auto list = std::vector<USBDevice>();
		RAWINPUTDEVICELIST* rawList = nullptr;
		UINT numDevices = 0;
		GetRawInputDeviceList(nullptr, &numDevices, sizeof(RAWINPUTDEVICELIST));
		rawList = new RAWINPUTDEVICELIST[numDevices];
		GetRawInputDeviceList(rawList, &numDevices, sizeof(RAWINPUTDEVICELIST));

		UINT bufferSize = 0;
		char* device = nullptr;
		for (int x = 0; x < numDevices; x++) {
			auto type = rawList[x].dwType;
			auto handle = rawList[x].hDevice;
			GetRawInputDeviceInfo(
				rawList[x].hDevice,
				RIDI_DEVICENAME,
				nullptr,
				&bufferSize
			);
			device = new char[bufferSize];
			memset(device, 0, bufferSize);
			GetRawInputDeviceInfo(
				rawList[x].hDevice,
				RIDI_DEVICENAME,
				device,
				&bufferSize
			);

			
			RID_DEVICE_INFO info;
			info.cbSize = sizeof(RID_DEVICE_INFO);
			bufferSize = sizeof(RID_DEVICE_INFO);
			GetRawInputDeviceInfo(
				rawList[x].hDevice,
				RIDI_DEVICEINFO,
				&info,
				&bufferSize
			);

			if(info.hid.usUsage != HID_USAGE_GENERIC_GAMEPAD) {
				continue;
			}

			list.emplace_back(USBDevice{
				device,
				static_cast<uint32_t>(info.hid.dwVendorId),
				static_cast<uint32_t>(info.hid.dwProductId)
				});
		}
		return list;
	}
}
