#pragma once
#include <cstdint>


namespace BrokenBytes::ControllerKit::Internal {
	enum class Rumble;
	class IRumbleController {
	public:
		virtual ~IRumbleController() = default;

		/// <summary>
		/// Sets the rumble of a given motor
		/// </summary>
		/// <param name="motor">The motor</param>
		/// <param name="strength">The intensity</param>
		virtual auto SetRumble(Rumble motor, uint8_t strength) -> void = 0;
	};
}
