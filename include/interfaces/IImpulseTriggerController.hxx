#pragma once

namespace BrokenBytes::ControllerKit::Internal {
	enum class Rumble;

	class IImpulseTriggerController {
	public:
		virtual ~IImpulseTriggerController() = default;

		virtual auto SetRumble(Rumble motor, uint8_t strength) -> void = 0;
		
	};
}
