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

#include <Types.h>

namespace BrokenBytes::ControllerKit {
	class GyroController {
	public:
		virtual ~GyroController() = default;
		[[nodiscard]] virtual auto GetGyroscope(
			ControllerKitButton button
		) const->ControllerKitGyroscope = 0;
		[[nodiscard]] virtual auto GetAcceleration(
			ControllerKitButton button
		) const->ControllerKitGyroscope = 0;
	};

	class TouchpadController {
	public:
		virtual ~TouchpadController() = default;
		[[nodiscard]] virtual auto GetTouches() const ->
			std::vector <ControllerKitTouch> = 0;
	};

	class LightbarController {
	public:
		virtual ~LightbarController() = default;
		virtual auto SetLightbarColor(ControllerKitColor color) const -> void = 0;
	};

	class AdaptiveTriggerController {
	public:
		virtual ~AdaptiveTriggerController() = default;
		virtual auto SetTriggerDisabled(ControllerKitTrigger trigger) -> void = 0;
		virtual auto SetTriggerContinuous(
			ControllerKitTrigger trigger,
			float start,
			float force
		) const -> void = 0;

		virtual auto SetTriggerSectional(
			ControllerKitTrigger trigger,
			float start,
			float end,
			float force
		) const -> void = 0;

		virtual auto SetTriggerAdvanced(
			ControllerKitTrigger trigger,
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
		virtual auto SetImpulseTrigger(ControllerKitRumble trigger, float strength) -> void = 0;
	};

	class Controller :
		public AdaptiveTriggerController,
		public LightbarController,
		public TouchpadController,
		public GyroController,
		public ImpulseTriggerController {
	public:
		Controller(uint8_t player, ControllerKitControllerType type);

		virtual ~Controller() = default;

		[[nodiscard]] virtual auto Player() const->uint8_t;
		[[nodiscard]] virtual auto Type() const->ControllerKitControllerType;

		[[nodiscard]] virtual auto HasFeature(
			uint8_t controller,
			ControllerKitFeature feature
		) const -> bool;

		[[nodiscard]] virtual auto GetButtonState(
			ControllerKitButton button
		) const->ControllerKitButtonState;

		[[nodiscard]] virtual auto GetAxis(
			ControllerKitAxis axis
		) const -> float;
		auto SetTriggerDisabled(ControllerKitTrigger trigger) -> void override;

		auto SetTriggerContinuous(ControllerKitTrigger trigger, float start, float force) const -> void override;

		auto SetTriggerSectional(ControllerKitTrigger trigger, float start, float end, float force) const -> void override;
		auto SetTriggerAdvanced(ControllerKitTrigger trigger, float extension, float strengthReleased, float strengthMiddle, float strengthPressed, float frequency, bool pauseOnPressed) const -> void override;
		auto SetLightbarColor(ControllerKitColor color) const -> void override;

		auto GetTouches() const->std::vector <ControllerKitTouch> override;

		[[nodiscard]] auto GetGyroscope(
			ControllerKitButton button
		) const->ControllerKitGyroscope override;

		[[nodiscard]] auto GetAcceleration(
			ControllerKitButton button
		) const->ControllerKitGyroscope override;

		auto SetImpulseTrigger(ControllerKitRumble trigger, float strength) -> void override;

	private:
		uint8_t _player;
		ControllerKitControllerType _type;
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
	DLL_EXPORT auto GetControllerType(int controller)->ControllerKitControllerType;
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
		std::function<void(uint8_t id, ControllerKitControllerType type)> controller
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
