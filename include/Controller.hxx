#pragma once
#include <functional>
#include <thread>
#include <map>

#include "Math.hxx"

namespace BrokenBytes::ControllerKit::Types {
	enum class Trigger;
	enum class ControllerType;
	enum class ButtonState;
	enum class Button;
	enum class DPadDirection;
}

namespace BrokenBytes::ControllerKit::Internal {
	struct InputReport {
		std::map<Types::Button, bool> Buttons;
		Math::Vector2<float> LeftStick;
		Math::Vector2<float> RightStick;
		float LeftTrigger;
		float RightTrigger;
		Types::DPadDirection DPad;
	};
	
	class Controller
	{
	public:
		Controller(Types::ControllerType type);
		Controller(Controller&) = delete;
		Controller(Controller&&) = delete;
		virtual ~Controller();
		Controller& operator=(const Controller&) = delete;
		Controller& operator=(Controller&&) = delete;

		[[nodiscard]] static auto Controllers() -> std::map<uint8_t, Controller*>;
		static auto OnControllerConnected(
			std::function<void(uint8_t id, Types::ControllerType type)> callback
		) -> void;
		static auto OnControllerDisconnected(
			std::function<void(uint8_t id)> callback
		) -> void;
		
		[[nodiscard]] virtual auto GetStick(uint8_t id) const -> Math::Vector2<float>;
		[[nodiscard]] virtual auto GetTrigger(Types::Trigger t) const -> float;
		[[nodiscard]] virtual auto GetDPadDirection() const -> Types::DPadDirection;
		[[nodiscard]] virtual auto GetButtonState(
			Types::Button button
		) const -> Types::ButtonState;
		[[nodiscard]] auto Type() const -> Types::ControllerType;

	protected:
		static inline std::map<uint8_t, Controller*> controllers;
		auto SetInputReport(InputReport report) -> void;

	private:
		static inline std::function<void(
			uint8_t id,
			Types::ControllerType type
		)> _onConnected;
		static inline std::function<void(uint8_t id)> _onDisconnected;
		InputReport _lastReport;
		InputReport _report;
		Types::ControllerType _type;
		uint8_t _number;
	};

}
