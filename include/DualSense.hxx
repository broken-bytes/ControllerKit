#pragma once

#include "HIDController.hxx"
#include "Types.hxx"
#include "interfaces/IGyroscopeController.hxx"
#include "interfaces/ILightbarController.hxx"
#include "interfaces/IRumbleController.hxx"
#include "interfaces/IAdaptiveTriggerController.hxx"
#include "interfaces/ITouchpadController.hxx"

namespace BrokenBytes::ControllerKit::Internal {
	constexpr uint8_t DUALSENSE_READ_REPORT_SIZE = 48;
	constexpr uint8_t DUALSENSE_WRITE_REPORT_SIZE = 32;
	
	class DualSense : public
		HIDController,
		IRumbleController,
		IGyroscopeController,
		ILightbarController,
		IAdaptiveTriggerController,
		ITouchpadController
	{
	public:
		DualSense(char* path);
		~DualSense();
		DualSense(const DualSense&) = delete;
		DualSense(const DualSense&&) = delete;
		DualSense& operator=(const DualSense&) = delete;
		DualSense& operator=(DualSense&&) = delete;
		bool operator==(char* path) const;
		
		auto ReadGyroscope() -> Math::Vector3<float> override;
		auto ReadAcceleration() -> Math::Vector3<float> override;
		auto SetLightbarColor(Types::Color c) -> void override;
		auto SetRumble(Rumble motor, uint8_t strength) -> void override;
		auto GetTouches()->std::vector<Math::Vector2<uint8_t>> override;
		auto SetTrigger(
			Types::Trigger trigger,
			Types::AdaptiveTriggerMode mode,
			Params params
		) -> void override;
	private:
		enum class Permission1 {
			Rumble = 0x03,
			RightTrigger = 0x04,
			LeftTrigger = 0x08,
			AudioVolume = 0x10,
			ToggleSpeakerExt = 0x20,
			MicVolume = 0x40,
			ToggleMicSpeakerInt = 0x80
		};

		enum class Permission2 {
			MicLED = 0x01,
			ToggleMute = 0x02,
			Lightbar = 0x04,
			ToggleAllLEDs = 0x08,
			TogglePlayerLED = 0x10,
			Unknown1 = 0x20,
			MotorPower = 0x40,
			Unknown2 = 0x80
		};
		
		std::thread _thread;
		bool _isDirty;
		uint8_t _reportPermissionFlags;
		unsigned char* _report;
		std::map<Types::Trigger, TriggerConfig> _triggers = {};
		Types::Color* _color;

		
		auto SetDirty() -> void;
		auto SetClear() -> void;
		auto SetPermission(Permission1 perm1, Permission2 perm2) const -> void;
		auto Routine() -> void;

		auto ParseRawInput(unsigned char* input) -> void;
	};
}
