#pragma once
#include <functional>

#include "Controller.hxx"

namespace BrokenBytes::ControllerKit::Interface {
	// Mainstream Vendor IDs
	constexpr uint16_t SONY = 0x054C;

	// Mainstream Controller PIDs
	constexpr uint16_t DS4_1 = 0x05C4;
	constexpr uint16_t DS4_2 = 0x09CC;
	constexpr uint16_t DS_1 = 0x0CE6;
	
	inline std::function<void(
		uint8_t id, Internal::Controller*
		)> ON_CONTROLLER_CONNECTED_EVENT;
	inline std::function<void(
		uint8_t id
		)> ON_CONTROLLER_DISCONNECTED_EVENT;

	auto Init() -> void;
	auto QueryDevices() -> void;
	auto OnControllerConnected(std::function<void(uint8_t id, Internal::Controller*)> controller) -> void;
	auto OnControllerDisconnected(std::function<void(uint8_t id)> controller) -> void;
	auto GetControllers() ->std::map<uint8_t, Internal::Controller*>;
	auto SetData(uint8_t controller, unsigned char* data) -> void;
}
