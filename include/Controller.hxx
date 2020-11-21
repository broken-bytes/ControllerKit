#pragma once

#include <cstdint>

namespace BrokenBytes::ControllerKit {
	enum ControllerType { DS4, DS5, XBoxOne, XBoxSeriesX };


	struct ButtonState {
		bool ActionUp;
		bool ActionRight;
		bool ActionDown;
		bool ActionLeft;
		bool DPadUp;
		bool DPadRight;
		bool DPadDown;
		bool DPadLeft;
		bool LeftShoulder;
		bool RightShoulder;
		bool LeftStick;
		// Overview on XBox One/Series X, Share on DS4/5
		bool Accessory1;
		// Menu on Xbox One/Series X, Start on DS4/5
		bool Accessory2;
		// Dead on XBox One/DS4, Share on XBox Series X, Mute on DS5
		bool Accessory3;
	};

	struct TriggerState {
		float LeftAmount;
		float RightAmount;
	};

	struct ControllerState {
		ButtonState ButtonState;
		TriggerState TriggerState;
		uint8_t Id;
	};

	class Controller
	{
	public:
		Controller(uint8_t id);
		ButtonState ButtonState() const;
		TriggerState TriggerState() const;
		ControllerType Type() const;
		void SetRumble(uint_fast8_t motorId, float strength);
	
	private:
		uint8_t _id;
		ControllerType _type;
	};
}