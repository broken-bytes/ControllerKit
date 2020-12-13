#pragma once

#pragma managed(push, off)
#include <ControllerKit.hxx>
using namespace BrokenBytes::ControllerKit::Types;
using namespace BrokenBytes::ControllerKit;
#pragma managed(pop)

#include <cliext/set>
#define DLL_EXPORT

namespace BrokenBytes::ControllerKit::Managed {
	public ref class Controller {
	public:
		Controller(uint8_t player) {
			this->_player = player;
		}

		static inline auto Controllers() {
			auto list = std::vector<uint8_t>();
			auto unmanaged = ControllerKit::Controllers();
			for (auto item : unmanaged) {
				list.emplace_back();
			}
		}

		auto GetAxis(Types::Axis axis) {
			return GetController().GetAxis(axis);
		}

		auto GetButtonState(Types::Button button) {
			return GetController().GetButtonState(button);
		}

		auto Type() {
			return GetController().Type();
		}

		auto SetLightbarColor(Types::Color color) {
			GetController().SetLightbarColor(color);
		}

		auto SetTriggerDisabled(Types::Trigger trigger) {
			GetController().SetTriggerDisabled(trigger);
		}

		auto SetTriggerContinuous(Types::Trigger trigger, float start, float force) {
			GetController().SetTriggerContinuous(trigger, start, force);
		}

		auto SetTriggerSectional(Types::Trigger trigger, float start, float end, float force) {
			GetController().SetTriggerSectional(trigger, start, end, force);
		}

		auto SetTriggerAdvanced(
			Types::Trigger trigger,
			float extension,
			float strengthReleased,
			float strengthMiddle,
			float strengthPressed,
			float frequency,
			bool pauseOnPressed) {

			GetController().SetTriggerAdvanced(
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
			GetController().SetImpulseTrigger(trigger, strength);	
		}

	private:
		uint8_t _player;

		auto GetController() -> BrokenBytes::ControllerKit::Controller {
			for (auto& item : ControllerKit::Controllers()) {
				if (item.Player() == _player) {
					return item;
				}
			}
		}
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