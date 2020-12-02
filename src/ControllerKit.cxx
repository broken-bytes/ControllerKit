#include <array>

#include "ControllerKit.hxx"
#include "Controller.hxx"
#include "Interface.hxx"
#include "USB.hxx"


using namespace BrokenBytes::ControllerKit;

namespace BrokenBytes::ControllerKit {
	int index = 0;
	auto controllers = std::array<Internal::Controller*, 16>();

	bool isInit = false;

	void Init() {
		Interface::Init();
	}

	
	bool HasFeature(uint8_t controller, Feature feature) {
		// TODO: Feature detection
		return true;
	}

	ButtonState GetButtonState(int controller, Button button) {
		return controllers[controller]->GetButtonState(button);
	}

	ControllerType GetControllerType(int controller) {
		return controllers[controller]->Type();
	}


	float GetAxis(int controller, Axis axis) {
		switch (axis) {
		case Axis::LeftX:
			return controllers[controller]->GetStick(0).X;
		case Axis::LeftY:
			return controllers[controller]->GetStick(0).Y;
		case Axis::RightX:
			return controllers[controller]->GetStick(1).X;
		case Axis::RightY:
			return controllers[controller]->GetStick(1).Y;
		case Axis::LeftTrigger:
			return controllers[controller]->GetTrigger(Trigger::Left);
		case Axis::RightTrigger:
			return controllers[controller]->GetTrigger(Trigger::Right);
		}
		return 0;
	}
}
