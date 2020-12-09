#pragma once

#pragma managed(push, off)
#include <ControllerKit.hxx>
using namespace BrokenBytes::ControllerKit::Types;
using namespace BrokenBytes::ControllerKit;
#pragma managed(pop)


#define DLL_EXPORT __declspec( dllexport )

namespace BrokenBytes::ControllerKit::Managed {
	class Controller {
	public:
		Controller(BrokenBytes::ControllerKit::Controller* c) {
			this->_controller = c;
		}

		~Controller() {
			delete _controller;
		}

		static inline auto Controllers() {
			auto list = std::vector<Managed::Controller*>();
			auto unmanaged = ControllerKit::Controllers();
			for (auto item : unmanaged) {
				list.emplace_back(new Managed::Controller(&item));
			}
		}


		auto GetAxis(Types::Axis axis) {
			return _controller->GetAxis(axis);
		}

		auto GetButtonState(Types::Button button) {
			return _controller->GetButtonState(button);
		}

		auto Type() {
			return _controller->Type();
		}

		auto SetLightbarColor(Types::Color color) {
			_controller->SetLightbarColor(color);
		}

		auto SetTriggerDisabled(Types::Trigger trigger) {
			_controller->SetTriggerDisabled(trigger);
		}

		auto SetTriggerContinuous(Types::Trigger trigger, float start, float force) {
			_controller->SetTriggerContinuous(trigger, start, force);
		}

		auto SetTriggerSectional(Types::Trigger trigger, float start, float end, float force) {
			_controller->SetTriggerSectional(trigger, start, end, force);
		}

		auto SetTriggerAdvanced(
			Types::Trigger trigger,
			float extension,
			float strengthReleased,
			float strengthMiddle,
			float strengthPressed,
			float frequency,
			bool pauseOnPressed) {
			_controller->SetTriggerAdvanced(
				trigger,
				extension,
				strengthReleased,
				strengthMiddle,
				strengthPressed,
				frequency,
				pauseOnPressed
			);
		}

		auto SetImpulseTrigger(Types::Trigger trigger, float strength) {
			_controller->SetImpulseTrigger(trigger, strength);
		}

	private:
		static inline std::vector<Managed::Controller> _controllers;
		BrokenBytes::ControllerKit::Controller* _controller;
	};

	DLL_EXPORT auto Init() {
		ControllerKit::Init();
	}

	DLL_EXPORT auto Next() {
		ControllerKit::Next();
	}

	DLL_EXPORT auto Flush() {
		ControllerKit::Flush();
	}

	DLL_EXPORT auto OnControllerConnected(std::function<void(uint8_t, ControllerType)> callback) {
		ControllerKit::OnControllerConnected(callback);
	}

	DLL_EXPORT auto OnControllerDisconnected(std::function<void(uint8_t)> callback) {
		ControllerKit::OnControllerDisconnected(callback);
	}

	BOOL WINAPI DllMain(
		HINSTANCE hinstDLL,  // handle to DLL module
		DWORD fdwReason,     // reason for calling function
		LPVOID lpReserved)  // reserved
	{
		// Perform actions based on the reason for calling.
		switch (fdwReason)
		{

		case DLL_PROCESS_ATTACH:
			break;

		case DLL_THREAD_ATTACH:
			// Do thread-specific initialization.
			break;

		case DLL_THREAD_DETACH:
			// Do thread-specific cleanup.
			break;

		case DLL_PROCESS_DETACH:
			// Perform any necessary cleanup.
			break;
		}
		return TRUE;  // Successful DLL_PROCESS_ATTACH.
	}
}