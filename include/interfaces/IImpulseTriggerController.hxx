#pragma once

#include <Types.h>

namespace BrokenBytes::ControllerKit::Internal {
	class IImpulseTriggerController {
	public:
		virtual ~IImpulseTriggerController() = default;
		virtual auto SetRumble(ControllerKitRumble motor, uint8_t strength) -> void = 0;	
	};
}
