#pragma once
#include <cstdint>
#include <Types.h>

namespace BrokenBytes::ControllerKit::Internal {
	class IRumbleController {
	public:
		virtual ~IRumbleController() = default;

		/// <summary>
		/// Sets the rumble of a given motor
		/// </summary>
		/// <param name="motor">The motor</param>
		/// <param name="strength">The intensity</param>
		virtual auto SetRumble(ControllerKitRumble motor, uint8_t strength) -> void = 0;
	};
}
