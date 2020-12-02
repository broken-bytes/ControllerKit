#include <vector>


#include "DualSense.hxx"
#include "DualShock4.hxx"

/// Defines Windows 10 Target
#define _W10

#ifdef _W10
#include <algorithm>
#include <concrt.h>
#include <ppl.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Gaming.Input.h>
using namespace winrt;
using namespace Windows::Gaming::Input;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
#else
#include "XInput.hxx"
#endif

#include "HID.hxx"
#include "USB.hxx"
#include "Interface.hxx"
#include "Controller.hxx"


using namespace BrokenBytes::ControllerKit::Internal;

namespace BrokenBytes::ControllerKit::Interface {
	struct ControllerDevice {
		USB::USBDevice* Handle;
		Controller* Controller;
	};

	std::vector<ControllerDevice> CONTROLLERS;
	std::thread WATCHDOG;
#ifdef  _W10
	Gamepad::GamepadAdded_revoker ADDED_REVOKER;
	Gamepad::GamepadRemoved_revoker REMOVED_REVOKER;

	void OnGamepadAdded(const IInspectable& sender, const Gamepad& gamepad) {
		std::cout << "Added" << std::endl;
	}

	void OnGamepadRemoved(const IInspectable& sender, const Gamepad& gamepad) {
		
	}
	
	void Init() {
		std::cout << "Init" << std::endl;
		USB::Init();
		auto pads = Gamepad::Gamepads();
		ADDED_REVOKER = Gamepad::GamepadAdded(
			auto_revoke,
			std::bind(
				&OnGamepadAdded,
				std::placeholders::_1,
				std::placeholders::_2
			)
		);
		REMOVED_REVOKER = Gamepad::GamepadRemoved(
			auto_revoke,
			std::bind(
				&OnGamepadRemoved,
				std::placeholders::_1,
				std::placeholders::_2
			)
		);


		IVectorView<Gamepad> gamepads = Gamepad::Gamepads();
		auto size = gamepads.Size();
		std::cout << size << std::endl;
		for (const Gamepad& gamepad : gamepads)
		{
			OnGamepadAdded(nullptr, gamepad);
		}
		
		WATCHDOG = std::thread([] {
			while (true) {
				QueryDevices();
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			});
	}
#endif

	auto QueryDevices() -> void {
		auto devices = USB::EnumerateDevices();
		for (int x = 0; x < CONTROLLERS.size(); x++) {
			bool contains = false;
			for (const auto& device : devices) {
				if (strcmp(device.Path, CONTROLLERS[x].Handle->Path) == 0) {
					contains = true;
					break;
				}
			}
			if (contains) {
				continue;
			}
			CONTROLLERS.erase(CONTROLLERS.begin() + x);
		}

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
					controller = new DualShock4(item.Path);
					continue;
				}
				// DualSense
				if (item.ProductId == DS_1) {
					controller = new DualSense(item.Path);
					continue;
				}
			}
			else {
				controller = new HIDController(item.Path, ControllerType::Generic);
			}

			CONTROLLERS.emplace_back(ControllerDevice{ &item, controller });
		}
	}

	void OnControllerConnected(std::function<void(Controller)> controller) {
		ON_CONTROLLER_CONNECTED_EVENT = controller;
	}

	void OnControllerDisconnected(std::function<void(Controller)> controller) {
		ON_CONTROLLER_DISCONNECTED_EVENT = controller;
	}

	std::vector<Controller*> GetControllers() {
		return std::vector<Controller*>();
	}

	void SetData(uint8_t controller, unsigned char* data) {}
}
