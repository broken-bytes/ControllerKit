#include <vector>
#ifdef _W10          // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
// Windows Header Files
#include <assert.h>
#include <concrt.h>
#include <cstdint>
#include <iostream>
#include <roapi.h>
#include <wrl.h>
#include <winrt/base.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Gaming.Input.h>
#include "Gaming.Input.hxx"
#include "GamingInputController.hxx"

using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::Gaming::Input;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
#pragma comment(lib, "runtimeobject.lib")
#else
#include "XInput.hxx"
#endif

#include "Interface.hxx"
#include "Controller.hxx"
#include "HID.hxx"
#include "USB.hxx"

#include "DualSense.hxx"
#include "DualShock4.hxx"



using namespace BrokenBytes::ControllerKit::Internal;

namespace BrokenBytes::ControllerKit::Interface {
	struct ControllerDevice {
		USB::USBDevice* Handle;
		Controller* Controller;
	};

	std::vector<ControllerDevice> HID_CONTROLLERS;
	std::thread WATCHDOG;
#ifdef _W10
	std::vector<GamingInputController*> W10_CONTROLLERS;
	concurrency::critical_section myLock{};

	auto OnGamepadAdded(const winrt::Windows::Foundation::IInspectable& sender, const Gamepad& gamepad) -> void {
		Controller::Create<GamingInputController>();
	}

	auto OnGamepadRemoved(const winrt::Windows::Foundation::IInspectable& sender, const Gamepad& gamepad) -> void {
		Controller::Remove<GamingInputController, Gamepad*>(&const_cast<Gamepad&>(gamepad));
	}

#endif

	void Init() {
#ifdef _W10
		GamingInput::Init();
		Gamepad::GamepadAdded(&OnGamepadAdded);
		Gamepad::GamepadRemoved(&OnGamepadRemoved);
#endif
		USB::Init();
		QueryDevices();
		
	}
	auto Flush() -> void {
		for (auto& item : Controller::Controllers()) {
			item.second->Flush();
		}
	}

	auto Next() -> void {
		for (auto& item : Controller::Controllers()) {
			item.second->Next();
		}
	}

	auto GetControllers() -> std::map<uint8_t, Controller*> {
		return Controller::Controllers();
	}



	auto OnControllerConnected(
		std::function<void(uint8_t id, Types::ControllerType type)> controller
	) -> void {
		Controller::OnControllerConnected(controller);
	}

	auto OnControllerDisconnected(std::function<void(uint8_t id)> controller) -> void {
		Controller::OnControllerDisconnected(controller);
	}

	auto QueryDevices() -> void {
		auto devices = USB::EnumerateDevices();

		for (auto item : devices) {
			if (std::wstring(item.Path).find(L"IG_") != std::string::npos) {
				// Skip XInput Controllers as we are using Windows.Gaming.Input or XInput and not A custom HID for them
				continue;
			}
			// Specific controllers check

			// Playstation Controllers
			if (item.VendorId == SONY) {
				// DualShock 4
				if (item.ProductId == DS4_1 || item.ProductId == DS4_2) {
					Controller::Create<DualShock4, DevicePath>(item.Path);
					continue;
				}
				// DualSense
				if (item.ProductId == DS_1) {
					Controller::Create<DualSense, DevicePath>(item.Path);
					continue;
				}
			}
		}
	}

	auto SetData(uint8_t controller, unsigned char* data) -> void {}
}
