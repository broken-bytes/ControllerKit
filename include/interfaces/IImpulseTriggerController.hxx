#pragma once

namespace BrokenBytes::ControllerKit::Types {
	enum class Rumble;
}

namespace BrokenBytes::ControllerKit::Internal {
	class IImpulseTriggerController {
	public:
		virtual ~IImpulseTriggerController() = default;
		virtual auto SetRumble(Types::Rumble motor, uint8_t strength) -> void = 0;	
	};
}
