#pragma once

#include <vector>

#include "DualSense.hxx"
#include "Controller.hxx"
#include "Types.hxx"
#include "interfaces/IAdaptiveTriggerController.hxx"
#include "interfaces/IGyroscopeController.hxx"
#include "interfaces/ITouchpadController.hxx"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#ifdef _LIB_DYNAMIC
#define DLL_EXPORT __declspec( dllexport )
#else
#define DLL_EXPORT 
#endif
#endif

namespace BrokenBytes::ControllerKit {
	class Controller {
	public:
		Controller(uint8_t player, Types::ControllerType type) {
			_player = player;
			_type = type;
		}

		virtual ~Controller() = default;

		[[nodiscard]] virtual auto Player() const ->uint8_t {
			return _player;
		}

		[[nodiscard]] virtual auto Type() const ->Types::ControllerType {
			return _type;
		}
		
		[[nodiscard]] virtual auto HasFeature(
			uint8_t controller,
			Types::Feature feature
		) const -> bool {
			// TODO: Feature detection
			return true;
		}

		[[nodiscard]] virtual auto GetButtonState(
			Types::Button button
		) const -> Types::ButtonState {
			return Interface::GetControllers()[_player]->GetButtonState(button);
		}

		[[nodiscard]] virtual auto GetAxis(
			Types::Axis axis
		) const -> float {
			switch (axis) {
			case Types::Axis::LeftX:
				return Interface::GetControllers()[_player]->GetStick(0).X;
			case Types::Axis::LeftY:
				return Interface::GetControllers()[_player]->GetStick(0).Y;
			case Types::Axis::RightX:
				return Interface::GetControllers()[_player]->GetStick(1).X;
			case Types::Axis::RightY:
				return Interface::GetControllers()[_player]->GetStick(1).Y;
			case Types::Axis::LeftTrigger:
				return Interface::GetControllers()[_player]->GetTrigger(Types::Trigger::Left);
			case Types::Axis::RightTrigger:
				return Interface::GetControllers()[_player]->GetTrigger(Types::Trigger::Right);
			}
			return 0;
		}

	private:
		uint8_t _player;
		Types::ControllerType _type;
	};

	class GyroController : Controller {
	public:
		GyroController(uint8_t player, Types::ControllerType type) :
			Controller(player, type) {}
		[[nodiscard]] auto GetGyroscope(
			Types::Button button
		) const -> Math::Vector3<float> {
			auto* c = dynamic_cast<Internal::IGyroscopeController*>(
				Interface::GetControllers()[Player()]
			);
			return c->ReadGyroscope();
		}
		[[nodiscard]] auto GetAcceleration(
			Types::Button button
		) const -> Math::Vector3<float> {
			auto* c = dynamic_cast<Internal::IGyroscopeController*>(
				Interface::GetControllers()[Player()]
				);
			return c->ReadAcceleration();
		}
	};

	class TouchpadController : Controller {
	public:
		TouchpadController(uint8_t player, Types::ControllerType type) :
			Controller(player, type) {}
		[[nodiscard]] auto GetTouches(
			Types::Button button
		) const -> std::vector<Math::Vector2<float>> {
			auto* c = dynamic_cast<Internal::ITouchpadController*>(
				Interface::GetControllers()[Player()]
				);
			return c->GetTouches();
		}
	};

	class AdaptiveTriggerController : Controller {
	public:
		AdaptiveTriggerController(uint8_t player, Types::ControllerType type) :
			Controller(player, type) {}
		auto SetTriggerContinuous(
			Types::Trigger trigger,
			float start,
			float force
		) const -> void {
			auto* raw = Interface::GetControllers()[Player()];
			Internal::IAdaptiveTriggerController::Params params{};
			params.ForceOrEnd = Math::ConvertToUnsignedShort(force);
			params.Start = Math::ConvertToUnsignedShort(start);
			if(Type() == Types::ControllerType::DualSense) {
				dynamic_cast<Internal::DualSense*>(raw)->SetTrigger(
					trigger,
					Types::AdaptiveTriggerMode::Continuous,
					params
				);
			}
		}
		auto SetTriggerContinuousAdvanced(
			Types::Trigger trigger,
			float start,
			float force,
			float strengthReleased,
			float strengthMiddle,
			float strengthPressed,
			float frequency,
			bool pauseOnPressed
		) const -> void {
			auto* raw = Interface::GetControllers()[Player()];
			Internal::IAdaptiveTriggerController::Params params{};
			params.ForceOrEnd = Math::ConvertToUnsignedShort(force);
			params.ForceOrEnd &= (pauseOnPressed) ? 0x00 : 0x02;
			params.Start = Math::ConvertToUnsignedShort(start);
			params.Strength = {
				Math::ConvertToUnsignedShort(strengthReleased),
				Math::ConvertToUnsignedShort(strengthMiddle),
				Math::ConvertToUnsignedShort(strengthPressed)
			};
			params.Frequency = Math::ConvertToUnsignedShort(frequency);
			if (Type() == Types::ControllerType::DualSense) {
				dynamic_cast<Internal::DualSense*>(raw)->SetTrigger(
					trigger,
					Types::AdaptiveTriggerMode::ContinuousAdvanced,
					params
				);
			}
		}

		auto SetTriggerSectional(
			Types::Trigger trigger,
			float start,
			float end,
			float force
		) const -> void {
			auto* raw = Interface::GetControllers()[Player()];
			Internal::IAdaptiveTriggerController::Params params{};
			params.ForceOrEnd = Math::ConvertToUnsignedShort(end);
			params.Start = Math::ConvertToUnsignedShort(start);
			params.ForceInRange = force;
			if (Type() == Types::ControllerType::DualSense) {
				dynamic_cast<Internal::DualSense*>(raw)->SetTrigger(
					trigger,
					Types::AdaptiveTriggerMode::Sectional,
					params
				);
			}
		}

		auto SetTriggerSectionalAdvanced(
			Types::Trigger trigger,
			float start,
			float end,
			float force,
			float strengthReleased,
			float strengthMiddle,
			float strengthPressed,
			float frequency,
			bool pauseOnPressed
		) const -> void {
			auto* raw = Interface::GetControllers()[Player()];
			Internal::IAdaptiveTriggerController::Params params{};
			params.ForceOrEnd = Math::ConvertToUnsignedShort(end);
			params.Start = Math::ConvertToUnsignedShort(start);
			params.ForceInRange = force;
			params.Strength = {
				Math::ConvertToUnsignedShort(strengthReleased),
				Math::ConvertToUnsignedShort(strengthMiddle),
				Math::ConvertToUnsignedShort(strengthPressed)
			};
			params.Frequency = Math::ConvertToUnsignedShort(frequency);
			if (Type() == Types::ControllerType::DualSense) {
				dynamic_cast<Internal::DualSense*>(raw)->SetTrigger(
					trigger,
					Types::AdaptiveTriggerMode::SectionalAdvanced,
					params
				);
			}
		}
	};

	DLL_EXPORT void Init();
	DLL_EXPORT auto Controllers()->std::vector<Controller>;
	DLL_EXPORT auto GetControllerType(int controller)-> Types::ControllerType;
	DLL_EXPORT auto OnControllerConnected(
		std::function<void(uint8_t id, Types::ControllerType type)> controller
	) -> void;
	DLL_EXPORT auto OnControllerDisconnected(
		std::function<void(uint8_t id)> controller
	) -> void;
#ifdef _WIN32
#ifdef _LIB_DYNAMIC
	BOOL WINAPI DllMain(
		_In_ HINSTANCE hinstDLL,
		_In_ DWORD     fdwReason,
		_In_ LPVOID    lpvReserved
	);
#endif
#endif
}
