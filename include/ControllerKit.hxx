#pragma once

#include <vector>

#include "DualSense.hxx"
#include "Controller.hxx"
#include "DualShock4.hxx"
#include "Types.hxx"
#include "interfaces/IAdaptiveTriggerController.hxx"
#include "interfaces/IGyroscopeController.hxx"
#include "interfaces/IImpulseTriggerController.hxx"
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
	class GyroController {
	public:
		virtual ~GyroController() = default;
		[[nodiscard]] virtual auto GetGyroscope(
			Types::Button button
		) const->Math::Vector3<float> = 0;
		[[nodiscard]] virtual auto GetAcceleration(
			Types::Button button
		) const->Math::Vector3<float> = 0;
	};

	class TouchpadController {
	public:
		virtual ~TouchpadController() = default;
		[[nodiscard]] virtual auto GetTouches(
			Types::Button button
		) const->std::vector<Math::Vector2<float>> = 0;
	};

	class LightbarController {
	public:
		virtual ~LightbarController() = default;
		virtual auto SetLightbarColor(Types::Color color) const -> void = 0;
	};

	class AdaptiveTriggerController {
	public:
		virtual ~AdaptiveTriggerController() = default;
		virtual auto SetTriggerDisabled(Types::Trigger trigger) -> void = 0;
		virtual auto SetTriggerContinuous(
			Types::Trigger trigger,
			float start,
			float force
		) const -> void = 0;

		virtual auto SetTriggerSectional(
			Types::Trigger trigger,
			float start,
			float end,
			float force
		) const -> void = 0;

		virtual auto SetTriggerAdvanced(
			Types::Trigger trigger,
			float extension,
			float strengthReleased,
			float strengthMiddle,
			float strengthPressed,
			float frequency,
			bool pauseOnPressed
		) const -> void = 0;
	};

	class Controller :
		public AdaptiveTriggerController,
		public LightbarController,
		public TouchpadController,
		public GyroController {
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
			auto raw = Interface::GetControllers()[_player];
			switch (feature) {
			case Types::Feature::Rumble:
				return true;
				break;
			case Types::Feature::Gyroscope:
				if(dynamic_cast<Internal::IGyroscopeController*>(raw)) {
					return true;
				}
				break;
			case Types::Feature::Lightbar:
				if (dynamic_cast<Internal::ILightbarController*>(raw)) {
					return true;
				}
				break;
			case Types::Feature::AdaptiveTriggers:
				if (dynamic_cast<Internal::IAdaptiveTriggerController*>(raw)) {
					return true;
				}
				break;
			case Types::Feature::ImpulseTriggers:
				if (dynamic_cast<Internal::IImpulseTriggerController*>(raw)) {
					return true;
				}
				break;
			case Types::Feature::Touchpad:
				if (dynamic_cast<Internal::ITouchpadController*>(raw)) {
					return true;
				}
				break;
			default: ;
			}
			return false;
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

		auto SetTriggerDisabled(Types::Trigger trigger) -> void override {
			auto* raw = Interface::GetControllers()[Player()];
			Internal::IAdaptiveTriggerController::Params params{
				0xFF,
				0xFF,
				0xFF,
				{
					0xFF,
					0xFF,
					0xFF
				},
				false,
				0xFF
			};
			if (Type() == Types::ControllerType::DualSense) {
				dynamic_cast<Internal::DualSense*>(raw)->SetTrigger(
					trigger,
					Types::AdaptiveTriggerMode::Disabled,
					params
				);
			}
		}

		auto SetTriggerContinuous(Types::Trigger trigger, float start, float force) const -> void override {
			auto* raw = Interface::GetControllers()[Player()];
			Internal::IAdaptiveTriggerController::Params params{};
			params.ForceOrEnd = Math::ConvertToUnsignedShort(force);
			params.Start = Math::ConvertToUnsignedShort(start);
			if (Type() == Types::ControllerType::DualSense) {
				dynamic_cast<Internal::DualSense*>(raw)->SetTrigger(
					trigger,
					Types::AdaptiveTriggerMode::Continuous,
					params
				);
			}
		}

		auto SetTriggerSectional(Types::Trigger trigger, float start, float end, float force) const -> void override {
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

		auto SetTriggerAdvanced(Types::Trigger trigger, float extension, float strengthReleased, float strengthMiddle, float strengthPressed, float frequency, bool pauseOnPressed) const -> void override {
			auto* raw = Interface::GetControllers()[Player()];
			Internal::IAdaptiveTriggerController::Params params{};
			params.ForceOrEnd = (pauseOnPressed) ? 0x00 : 0x02;
			params.Start = Math::ConvertToUnsignedShort(extension);
			params.ForceInRange = 0x00;
			params.Strength = {
				Math::ConvertToUnsignedShort(strengthReleased),
				Math::ConvertToUnsignedShort(strengthMiddle),
				Math::ConvertToUnsignedShort(strengthPressed)
			};
			params.Frequency = Math::ConvertToUnsignedShort(frequency);
			if (Type() == Types::ControllerType::DualSense) {
				dynamic_cast<Internal::DualSense*>(raw)->SetTrigger(
					trigger,
					Types::AdaptiveTriggerMode::Advanced,
					params
				);
			}
		}

		auto SetLightbarColor(Types::Color color) const -> void override {
			auto raw = Interface::GetControllers()[Player()];
			if(Type() == Types::ControllerType::DualSense) {
				dynamic_cast<Internal::DualSense*>(raw)->SetLightbarColor(color);
			}
			if(Type() == Types::ControllerType::DualShock4) {
				dynamic_cast<Internal::DualShock4*>(raw)->SetLightbarColor(color);
			}
		}

		auto GetTouches(Types::Button button) const -> std::vector<Math::Vector2<float>> override {
			auto* c = dynamic_cast<Internal::ITouchpadController*>(
				Interface::GetControllers()[Player()]
				);
			return c->GetTouches();
		}

		[[nodiscard]] auto GetGyroscope(
			Types::Button button
		) const -> Math::Vector3<float> override {
			auto* c = dynamic_cast<Internal::IGyroscopeController*>(
				Interface::GetControllers()[Player()]
				);
			return c->ReadGyroscope();
		}

		[[nodiscard]] auto GetAcceleration(
			Types::Button button
		) const -> Math::Vector3<float> override {
			auto* c = dynamic_cast<Internal::IGyroscopeController*>(
				Interface::GetControllers()[Player()]
				);
			return c->ReadAcceleration();
		}
		
	private:
		uint8_t _player;
		Types::ControllerType _type;
	};

	DLL_EXPORT void Init();
	DLL_EXPORT auto Controllers()->std::vector<Controller>;
	DLL_EXPORT auto GetControllerType(int controller)->Types::ControllerType;
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
