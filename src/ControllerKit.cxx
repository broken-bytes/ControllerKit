#include <array>

#include "ControllerKit.hxx"

#include <minwindef.h>

#include "Controller.hxx"
#include "Interface.hxx"
#include "USB.hxx"


using namespace BrokenBytes::ControllerKit;
using namespace BrokenBytes::ControllerKit::Types;

namespace BrokenBytes::ControllerKit {
	int index = 0;
	auto controllers = std::array<Internal::Controller*, 16>();

	bool isInit = false;

	void Init() {
		Interface::Init();
	}

	auto Controllers() -> std::vector<Types::Controller> {
		std::vector<Types::Controller> list;
		auto controllers = Interface::GetControllers();
		for(auto item: controllers) {
			list.emplace_back(Types::Controller{ item.first, item.second->Type() });
		}
		return list;
	}

	auto OnControllerConnected(std::function<void(uint8_t id, ControllerType type)> controller) -> void {
		Interface::OnControllerConnected([controller](uint8_t id, Internal::Controller* c)->void {
			controller(id, c->Type());
			});
	}
	auto OnControllerDisconnected(std::function<void(uint8_t id)> controller) -> void {
		Interface::OnControllerDisconnected([controller](uint8_t id) {
			controller(id);
		});
	}
}

#ifdef _WIN32
#ifdef _LIB_DYNAMIC
BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpReserved)  // reserved
{
	// Perform actions based on the reason for calling.
	switch (fdwReason)
	{
		
	case DLL_PROCESS_ATTACH:
		Init();
		break;

	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;

	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;

	case DLL_PROCESS_DETACH:
		// Perform any necessary cleanup.
		break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
#endif
#endif