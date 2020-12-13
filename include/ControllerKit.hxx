#pragma once

#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <functional>
#ifdef _LIB_DYNAMIC
#define DLL_EXPORT __declspec( dllexport )
#else
#define DLL_EXPORT 
#endif
#endif

namespace BrokenBytes::ControllerKit::Types {
	struct Color {
		uint8_t R;
		uint8_t G;
		uint8_t B;
	};

	struct Stick {
		float X;
		float Y;
	};

	struct Touch {
		float X;
		float Y;
		uint8_t Index;
	};

	struct Gyroscope {
		float X;
		float Y;
		float Z;
	};

	enum class Feature {
		Rumble,
		Gyroscope,
		Lightbar,
		AdaptiveTriggers,
		ImpulseTriggers,
		Touchpad
	};

	enum class ButtonState {
		Down,
		Pressed,
		Up,
		Released
	};

	enum class DPadDirection {
		None,
		Left = 1,
		Up = 2,
		LeftUp = 3,
		Right = 4,
		RightUp = 6,
		Down = 8,
		LeftDown = 9,
		RightDown = 12
	};

	enum class Trigger {
		Left,
		Right
	};

	enum class Rumble {
		Left,
		Right,
		TriggerLeft,
		TriggerRight
	};

	enum class ControllerType {
		XBoxOne,
		XBoxSeries,
		DualShock4,
		DualSense,
		Generic
	};

	enum class Axis {
		LeftX,
		LeftY,
		RightX,
		RightY,
		LeftTrigger,
		RightTrigger
	};

	enum class Button {
		Button0, Cross = 0, A = 0,
		Button1, Square = 1, X = 1,
		Button2, Circle = 2, B = 2,
		Button3, Triangle = 3, Y = 3,
		Button4, L1 = 4, LB = 4,
		Button5, R1 = 5, RB = 5,
		Button6, L2 = 6, LT = 6,
		Button7, R2 = 7, RT = 7,
		Button8, Share = 8, Create = 8, View = 8,
		Button9, Options = 9, Menu = 9,
		Button10, L3 = 10,
		Button11, R3 = 11,
		Button12, PS = 12, XBox = 12,
		Button13, TouchPad = 13, XBox_Share = 13,
		Button14, PS_MUTE = 14,
		Button15 = 15,
		Button16 = 16
	};

	enum class AdaptiveTriggerMode {
		Disabled = 0x00,
		Continuous = 0x01,
		Sectional = 0x02,
		Advanced = 0x26
	};
}

namespace BrokenBytes::ControllerKit {
	class GyroController {
	public:
		virtual ~GyroController() = default;
		[[nodiscard]] virtual auto GetGyroscope(
			Types::Button button
		) const->Types::Gyroscope = 0;
		[[nodiscard]] virtual auto GetAcceleration(
			Types::Button button
		) const->Types::Gyroscope = 0;
	};

	class TouchpadController {
	public:
		virtual ~TouchpadController() = default;
		[[nodiscard]] virtual auto GetTouches() const -> 
			std::vector <Types::Touch> = 0;
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

	class ImpulseTriggerController {
	public:
		virtual auto SetImpulseTrigger(Types::Trigger trigger, float strength) -> void = 0;
	};

	class Controller :
		public AdaptiveTriggerController,
		public LightbarController,
		public TouchpadController,
		public GyroController,
		public ImpulseTriggerController {
	public:
		Controller(uint8_t player, Types::ControllerType type);

		virtual ~Controller() = default;

		[[nodiscard]] virtual auto Player() const->uint8_t;
		[[nodiscard]] virtual auto Type() const->Types::ControllerType;

		[[nodiscard]] virtual auto HasFeature(
			uint8_t controller,
			Types::Feature feature
		) const -> bool;

		[[nodiscard]] virtual auto GetButtonState(
			Types::Button button
		) const->Types::ButtonState;

		[[nodiscard]] virtual auto GetAxis(
			Types::Axis axis
		) const -> float;
		auto SetTriggerDisabled(Types::Trigger trigger) -> void override;

		auto SetTriggerContinuous(Types::Trigger trigger, float start, float force) const -> void override;

		auto SetTriggerSectional(Types::Trigger trigger, float start, float end, float force) const -> void override;
		auto SetTriggerAdvanced(Types::Trigger trigger, float extension, float strengthReleased, float strengthMiddle, float strengthPressed, float frequency, bool pauseOnPressed) const -> void override;
		auto SetLightbarColor(Types::Color color) const -> void override;

		auto GetTouches() const->std::vector <Types::Touch> override;

		[[nodiscard]] auto GetGyroscope(
			Types::Button button
		) const->Types::Gyroscope override;

		[[nodiscard]] auto GetAcceleration(
			Types::Button button
		) const->Types::Gyroscope override;

		auto SetImpulseTrigger(Types::Trigger trigger, float strength) -> void override;

	private:
		uint8_t _player;
		Types::ControllerType _type;
	};

	/// <summary>
	/// Initializes the library
	/// </summary>
	/// <returns></returns>
	DLL_EXPORT void Init();
	/// <summary>
	/// Gets the connected controllers
	/// </summary>
	/// <returns>The list of controllers</returns>
	DLL_EXPORT auto Controllers()->std::vector<Controller>;
	DLL_EXPORT auto GetControllerType(int controller)->Types::ControllerType;
	/// <summary>
	/// Clears the input queue
	/// </summary>
	/// <returns></returns>
	DLL_EXPORT auto Flush() -> void;
	/// <summary>
	/// Moves the input queue one forward, removing the current entry
	/// </summary>
	/// <returns></returns>
	DLL_EXPORT auto Next() -> void;
	/// <summary>
	/// Assigns the connected event callback
	/// </summary>
	/// <param name="controller"></param>
	/// <returns></returns>
	DLL_EXPORT auto OnControllerConnected(
		std::function<void(uint8_t id, Types::ControllerType type)> controller
	) -> void;
	/// <summary>
	/// Assigns the disconnected event callback
	/// </summary>
	/// <param name="controller"></param>
	/// <returns></returns>
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
