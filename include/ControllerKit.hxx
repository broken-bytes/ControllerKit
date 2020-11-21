#pragma once
#pragma(lib, "hid.lib")

#include <functional>


namespace BrokenBytes::ControllerKit {
	class Controller;
	void Init();
	void Deinit();
	void SetConnectHook(std::function<void(Controller)> callback);
	void SetDisconnectHookHook(std::function<void(Controller)> callback);
}