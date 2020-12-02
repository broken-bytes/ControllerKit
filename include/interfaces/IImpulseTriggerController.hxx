#pragma once

namespace BrokenBytes::ControllerKit::Internal {
	enum class Rumble;

	class IImpulseTriggerController {
	public:
		virtual ~IImpulseTriggerController() = default;

		virtual auto SetRumble(Rumble r) -> void = 0;
		
	};
}
