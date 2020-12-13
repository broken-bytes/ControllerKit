#include <array>
#include <utility>

#include "ControllerKit.hxx"
#include <minwindef.h>

#include <DualSense.hxx>
#include <Controller.hxx>
#include <DualShock4.hxx>
#ifdef _W10
#include <GamingInputController.hxx>
#else
#include <XInputController.hxx>
#endif
#include <GamingInputController.hxx>
#include <interfaces/IAdaptiveTriggerController.hxx>
#include <interfaces/IGyroscopeController.hxx>
#include <interfaces/IImpulseTriggerController.hxx>
#include <interfaces/ITouchpadController.hxx>
#include "Controller.hxx"
#include "Interface.hxx"
#include "USB.hxx"


using namespace BrokenBytes::ControllerKit;
using namespace Types;

namespace BrokenBytes::ControllerKit {
	Controller::Controller(uint8_t player, Types::ControllerType type) {
		_player = player;
		_type = type;
	}

	Controller::~Controller() = default;

	[[nodiscard]] auto Controller::Player() const ->uint8_t {
		return _player;
	}

	[[nodiscard]] auto Controller::Type() const ->Types::ControllerType {
		return _type;
	}

	[[nodiscard]] auto Controller::HasFeature(
		uint8_t controller,
		Types::Feature feature
	) const -> bool {
		auto raw = Interface::GetControllers()[_player];
		switch (feature) {
		case Types::Feature::Rumble:
			return true;
			break;
		case Types::Feature::Gyroscope:
			if (dynamic_cast<Internal::IGyroscopeController*>(raw)) {
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
		default:;
		}
		return false;
	}

	[[nodiscard]] auto Controller::GetButtonState(
		Types::Button button
	) const -> Types::ButtonState {
		return Interface::GetControllers()[_player]->GetButtonState(button);
	}

	[[nodiscard]] auto Controller::GetAxis(
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

	auto Controller::SetTriggerDisabled(Types::Trigger trigger) -> void {
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

	auto Controller::SetTriggerContinuous(Types::Trigger trigger, float start, float force) const -> void {
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

	auto Controller::SetTriggerSectional(Types::Trigger trigger, float start, float end, float force) const -> void {
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

	auto Controller::SetTriggerAdvanced(Types::Trigger trigger, float extension, float strengthReleased, float strengthMiddle, float strengthPressed, float frequency, bool pauseOnPressed) const -> void {
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

	auto Controller::SetLightbarColor(Types::Color color) const -> void {
		auto raw = Interface::GetControllers()[Player()];
		if (Type() == Types::ControllerType::DualSense) {
			dynamic_cast<Internal::DualSense*>(raw)->SetLightbarColor(color);
		}
		if (Type() == Types::ControllerType::DualShock4) {
			dynamic_cast<Internal::DualShock4*>(raw)->SetLightbarColor(color);
		}
	}

	[[nodiscard]] auto Controller::GetTouches() const -> std::vector<Types::Touch> {
		auto* c = dynamic_cast<Internal::ITouchpadController*>(
			Interface::GetControllers()[Player()]
			);
		auto touches = c->GetTouches();
		return { 
			{ touches[0].X, touches[0].Y, 0 },
			{ touches[1].X, touches[1].Y, 1 }
		};
	}

	[[nodiscard]] auto Controller::GetGyroscope(
		Types::Button button
	) const -> Types::Gyroscope {
		auto* c = dynamic_cast<Internal::IGyroscopeController*>(
			Interface::GetControllers()[Player()]
			);
		auto gyro = c->ReadGyroscope();
		return { gyro.X, gyro.Y, gyro.Z };
	}

	[[nodiscard]] auto Controller::GetAcceleration(
		Types::Button button
	) const -> Types::Gyroscope {
		auto* c = dynamic_cast<Internal::IGyroscopeController*>(
			Interface::GetControllers()[Player()]
			);
		auto gyro = c->ReadGyroscope();
		return { gyro.X, gyro.Y, gyro.Z };
	}

	auto Controller::SetImpulseTrigger(Types::Trigger trigger, float strength) -> void {
		auto raw = Interface::GetControllers()[Player()];
		if (Type() == Types::ControllerType::XBoxOne ||
			Type() == Types::ControllerType::XBoxSeries
			) {
			auto motor = (trigger == Types::Trigger::Left) ?
				Types::Rumble::TriggerLeft :
				Types::Rumble::TriggerRight;
			reinterpret_cast<Internal::GamingInputController*>(raw)->SetRumble(
				motor,
				Math::ConvertToUnsignedShort(strength)
			);
		}
	}

	void Init() {
		Interface::Init();
	}

	auto Controllers() -> std::vector<Controller> {
		std::vector<Controller> list;
		auto controllers = Interface::GetControllers();
		for (auto item : controllers) {
			list.emplace_back(Controller{ item.first, item.second->Type() });
		}
		return list;
	}

	auto GetControllerType(int controller) -> Types::ControllerType {
		return Interface::GetControllers()[controller]->Type();
	}

	auto Flush() -> void {
		Interface::Flush();
	}

	auto Next() -> void {
		Interface::Next();
	}

	auto OnControllerConnected(std::function<void(uint8_t id, ControllerType type)> controller) -> void {
		Interface::OnControllerConnected(std::move(controller));
	}
	auto OnControllerDisconnected(std::function<void(uint8_t id)> controller) -> void {
		Interface::OnControllerDisconnected(std::move(controller));
	}
}

#ifdef _WIN32
#ifdef _LIB_DYNAMIC
BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpReserved)  // reserved
{
	// Perform actions based on the reason for calling.
	switch (fdwReason)
	{

	case DLL_PROCESS_ATTACH:
		Init();
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
#endif
#endif