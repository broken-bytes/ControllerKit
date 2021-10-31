#pragma once
#include <functional>
#include <thread>
#include <map>
#include <queue>
#include <type_traits>
#include <typeinfo>

#include "Math.hxx"
#include <Types.h>

namespace BrokenBytes::ControllerKit::Internal {
	struct InputReport {
		std::map<uint8_t, uint8_t> Buttons;
		Math::Vector2<uint8_t> LeftStick;
		Math::Vector2<uint8_t> RightStick;
		uint8_t LeftTrigger;
		uint8_t RightTrigger;
		uint8_t DPad;
	};
	
	class Controller
	{
	public:
		Controller(ControllerKitControllerType type);
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
			std::function<void(uint8_t id, ControllerKitControllerType type)> callback
		) -> void;
		static auto OnControllerDisconnected(
			std::function<void(uint8_t id)> callback
		) -> void;

		virtual auto Flush() -> void;
		virtual auto Next() -> void;

		[[nodiscard]] virtual auto GetStick(uint8_t id) const->Math::Vector2<float>;
		[[nodiscard]] virtual auto GetTrigger(ControllerKitTrigger t) const -> float;
		[[nodiscard]] virtual auto GetDPadDirection() const->ControllerKitDPadDirection;
		[[nodiscard]] virtual auto GetButtonState(
			ControllerKitButton button
		) const->ControllerKitButtonState;
		[[nodiscard]] auto Type() const->ControllerKitControllerType;

	protected:
		static inline std::map<uint8_t, Controller*> controllers;
		auto SetInputReport(InputReport report) -> void;

	private:
		static inline std::function<void(
			uint8_t id,
			ControllerKitControllerType type
			)> _OnConnected;
		static inline std::function<void(uint8_t id)> _OnDisconnected;
		std::queue<InputReport> _queue;
		ControllerKitControllerType _type;
		uint8_t _number;

		static auto Add(Controller* controller) -> void;
	};

}
