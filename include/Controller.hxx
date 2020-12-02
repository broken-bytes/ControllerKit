#pragma once
#include <thread>
#include <map>

#include "ControllerKit.hxx"
#include "Math.hxx"

using namespace BrokenBytes::ControllerKit::Math;


namespace BrokenBytes::ControllerKit {
	enum class ControllerType;
	enum class ButtonState;
}

namespace BrokenBytes::ControllerKit::Internal {
	struct InputReport {
		std::map<Button, bool> Buttons;
		Vector2<float> LeftStick;
		Vector2<float> RightStick;
		float LeftTrigger;
		float RightTrigger;
		DPadDirection DPad;
	};
	

	class Controller
	{
	public:
		Controller(ControllerType type);
		Controller(Controller&) = delete;
		Controller(Controller&&) = delete;
		virtual ~Controller() = default;
		Controller& operator=(const Controller&) = delete;
		Controller& operator=(Controller&&) = delete;

		
		[[nodiscard]] virtual Vector2<float> GetStick(uint8_t id) const;
		[[nodiscard]] virtual float GetTrigger(Trigger t) const;
		[[nodiscard]] virtual DPadDirection GetDPadDirection() const;
		[[nodiscard]] virtual ButtonState GetButtonState(Button button) const;
		[[nodiscard]] ControllerType Type() const;

	protected:
		void SetInputReport(InputReport report);

	private:
		InputReport _lastReport;
		InputReport _report;
		ControllerType _type;
	};

}
