#include <vector>
#include <winrt/base.h>

#include "GamingInputController.hxx"

/// Defines Windows 10 Target
#define _W10

#ifdef _W10          // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
// Windows Header Files
#include <assert.h>
#include <concrt.h>
#include <cstdint>
#include <iostream>
#include <roapi.h>
#include <wrl.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Gaming.Input.h>

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
#ifdef  _W10
	std::vector<GamingInputController*> W10_CONTROLLERS;
	concurrency::critical_section myLock{};


	auto GetGamepads() -> void {
	}
	
	auto OnGamepadAdded(const winrt::Windows::Foundation::IInspectable& sender, const Gamepad& gamepad) -> void {
		W10_CONTROLLERS.emplace_back(new GamingInputController(&gamepad));
	}

	auto OnGamepadRemoved(const winrt::Windows::Foundation::IInspectable& sender, const Gamepad& gamepad) -> void {
		for(auto& item: W10_CONTROLLERS) {
			if (item->Gamepad() == &gamepad) {
				delete item;
			}
		}
		W10_CONTROLLERS.emplace_back(new GamingInputController(&gamepad));
	}
	
	void Init() {
		USB::Init();

		WATCHDOG = std::thread([] {

			Gamepad::GamepadAdded(&OnGamepadAdded);
			Gamepad::GamepadRemoved(&OnGamepadRemoved);

			GetGamepads();

			while (true) {
				
			}
			});
	}
#endif

	auto QueryDevices() -> void {
		auto devices = USB::EnumerateDevices();

		for (auto item : devices) {
			if (std::string(item.Path).find("IG_") != std::string::npos) {
				// Skip XInput Controllers as we are using Windows.Gaming.Input or XInput and not A custom HID for them
				continue;
			}
			// Specific controllers check

			Controller* controller;

			// Playstation Controllers
			if (item.VendorId == SONY) {
				// DualShock 4
				if (item.ProductId == DS4_1 || item.ProductId == DS4_2) {
					controller = DualShock4::Create(item.Path);
					continue;
				}
				// DualSense
				if (item.ProductId == DS_1) {
					controller = DualSense::Create(item.Path);
					continue;
				}
			}
		}
	}

	auto OnControllerConnected(std::function<void(uint8_t id, Controller*)> controller) -> void {
		ON_CONTROLLER_CONNECTED_EVENT = controller;
	}

	auto OnControllerDisconnected(std::function<void(uint8_t id)> controller) -> void {
		ON_CONTROLLER_DISCONNECTED_EVENT = controller;
	}

	auto GetControllers() -> std::map<uint8_t, Controller*> {
		return Controller::Controllers();
	}

	auto SetData(uint8_t controller, unsigned char* data) -> void {}
}
