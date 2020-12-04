#pragma once

namespace BrokenBytes::ControllerKit::Internal {
	class IAdaptiveTriggerController {
	public:
		struct Strength {
			uint8_t Released;
			uint8_t Middle;
			uint8_t Pressed;
		};

		struct Params {
			uint8_t Start;
			uint8_t ForceOrEnd;
			uint8_t ForceInRange;
			Strength Strength;
			bool PauseOnPressed;
		};

		struct TriggerConfig {
			Types::AdaptiveTriggerMode Mode;
			Params Params;
		};

		virtual ~IAdaptiveTriggerController() = default;

		virtual auto SetTrigger(
			Types::Trigger trigger,
			Types::AdaptiveTriggerMode mode, Params params
		) -> void = 0;
	};
}
