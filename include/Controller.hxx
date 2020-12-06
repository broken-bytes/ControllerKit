#pragma once
#include <functional>
#include <thread>
#include <map>
#include <type_traits>
#include <typeinfo>

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
		[[nodiscard]] virtual auto Equals(void* data) -> bool = 0;
		

		[[nodiscard]] static auto Controllers()->std::map<uint8_t, Controller*>;
		template<typename T,
			typename D,
			typename = std::enable_if_t<std::is_base_of_v<Controller, T>>
		>
			static auto Create(D data) -> T* {
			for (auto item : controllers) {
				if (typeid(*item.second) == typeid(T)) {
					auto* ds = dynamic_cast<T*>(item.second);
					if (ds->Equals(reinterpret_cast<void*>(data))) {
						return ds;
					}
				}
			}
			auto ds = new T(data);

			Add(dynamic_cast<Controller*>(ds));
			return ds;
		}

		template<typename T,
			typename = std::enable_if_t<std::is_base_of_v<Controller, T>>
		>
			static auto Create() -> T* {
			auto ds = new T();
			Add(reinterpret_cast<Controller*>(ds));
			return ds;
		}

		template<typename T,
			typename D,
			typename = std::enable_if_t<std::is_base_of_v<Controller, T>>
		>
		static auto Remove(D data) -> void {
			for (auto item : Controller::controllers) {
				if (typeid(*item.second) == typeid(T)) {
					auto* device = reinterpret_cast<T*>(item.second);
					if (device->Equals(reinterpret_cast<void*>(data))) {
						delete controllers[item.first];
						_OnDisconnected(item.first);
						break;
					}
				}
			}
		}

		static auto OnControllerConnected(
			std::function<void(uint8_t id, Types::ControllerType type)> callback
		) -> void;
		static auto OnControllerDisconnected(
			std::function<void(uint8_t id)> callback
		) -> void;

		[[nodiscard]] virtual auto GetStick(uint8_t id) const->Math::Vector2<float>;
		[[nodiscard]] virtual auto GetTrigger(Types::Trigger t) const -> float;
		[[nodiscard]] virtual auto GetDPadDirection() const->Types::DPadDirection;
		[[nodiscard]] virtual auto GetButtonState(
			Types::Button button
		) const->Types::ButtonState;
		[[nodiscard]] auto Type() const->Types::ControllerType;

	protected:
		static inline std::map<uint8_t, Controller*> controllers;
		auto SetInputReport(InputReport report) -> void;

	private:
		static inline std::function<void(
			uint8_t id,
			Types::ControllerType type
			)> _OnConnected;
		static inline std::function<void(uint8_t id)> _OnDisconnected;
		InputReport _lastReport;
		InputReport _report;
		Types::ControllerType _type;
		uint8_t _number;

		static auto Add(Controller* controller) -> void;
	};

}
