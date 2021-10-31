#pragma once

namespace BrokenBytes::ControllerKit::Types {
	enum class AdaptiveTriggerMode;
}

namespace BrokenBytes::ControllerKit::Internal {
	class IAdaptiveTriggerController {
	public:
		struct Strength {
			uint8_t Released;
			uint8_t Middle;
			uint8_t Pressed;
		};

		struct Params {
			/// <summary>
			/// right trigger start of resistance section 0-255 (0 = released state; 0xb0 roughly matches trigger value 0xff); in mode SectionalAdvanced(0x26) this field has something to do with motor re-extension after a press-
			/// </summary>
			uint8_t Start;
			/// <summary>
			/// Continuous mode amount of force exerted; 0-255
			// Sectional mode end of resistance section (>= begin of resistance section is enforced); 0xff makes it behave like Continuous
			// Advanced mode flag(s?) 0x02 = do not pause effect when fully pressed
			/// </summary>
			uint8_t ForceOrEnd;
			/// <summary>
			/// the force while the trigger is in range
			/// </summary>
			uint8_t ForceInRange;
			Strength Strength;
			/// <summary>
			/// Pauses teh effect when fully pressed
			/// </summary>
			bool PauseOnPressed;
			/// <summary>
			/// The Frequency of the effect in Hz, limitations apply, not all 255Hz values are possible
			/// </summary>
			uint8_t Frequency;
		};

		struct TriggerConfig {
			ControllerKitAdaptiveTriggerMode Mode;
			Params Params;
		};

		virtual ~IAdaptiveTriggerController() = default;

		virtual auto SetTrigger(
			ControllerKitTrigger trigger,
			ControllerKitAdaptiveTriggerMode mode, Params params
		) -> void = 0;
	};
}
