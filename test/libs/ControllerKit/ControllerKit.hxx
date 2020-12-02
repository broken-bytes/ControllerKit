#pragma once

#include <functional>

class Controller;

namespace BrokenBytes::ControllerKit {
	void Init();
	void Deinit();
	void SetConnectHook(std::function<void(Controller)> callback);
	void SetDisconnectHookHook(std::function<void(Controller)> callback);
}