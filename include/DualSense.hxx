#pragma once

#include "HIDController.hxx"
#include "interfaces/IGyroscopeController.hxx"
#include "interfaces/ILightbarController.hxx"
#include "interfaces/IRumbleController.hxx"
#include "interfaces/IAdaptiveTriggerController.hxx"
#include "interfaces/ITouchpadController.hxx"


namespace BrokenBytes::ControllerKit::Internal {
	constexpr uint8_t DUALSENSE_READ_REPORT_SIZE = 64;
	constexpr uint8_t DUALSENSE_WRITE_REPORT_SIZE = 48;
	
	class DualSense :
	public HIDController,
	public IRumbleController,
	public IGyroscopeController,
	public ILightbarController,
	public IAdaptiveTriggerController,
	public ITouchpadController
	{
	public:
		DualSense(DevicePath path);
		virtual ~DualSense();
		DualSense(const DualSense&) = delete;
		DualSense(const DualSense&&) = delete;
		DualSense& operator=(const DualSense&) = delete;
		DualSense& operator=(DualSense&&) = delete;

		auto SetPlayerIndicator(uint8_t player) -> void;
		
		auto ReadGyroscope() -> Math::Vector3<float> override;
		auto ReadAcceleration() -> Math::Vector3<float> override;
		auto SetLightbarColor(ControllerKitColor c) -> void override;
		auto SetRumble(ControllerKitRumble motor, uint8_t strength) -> void override;
		auto GetTouches()->std::vector<Math::Vector2<float>> override;
		auto SetTrigger(
			ControllerKitTrigger trigger,
			ControllerKitAdaptiveTriggerMode mode,
			Params params
		) -> void override;
	private:
		enum class Permission1 {
			None = 0x00,
			Rumble = 0x03,
			RightTrigger = 0x04,
			LeftTrigger = 0x08,
			AudioVolume = 0x10,
			ToggleSpeakerExt = 0x20,
			MicVolume = 0x40,
			ToggleMicSpeakerInt = 0x80
		};

		enum class Permission2 {
			None = 0x00,
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
		uint8_t _reportPermissionFlags;
		unsigned char* _report;
		ControllerKitColor* _color;

		auto SetDirty() -> void override;
		auto SetClear() -> void override;
		
		auto SetPermission(Permission1 perm1, Permission2 perm2) const -> void;
		auto Routine() -> void;
	};
}
