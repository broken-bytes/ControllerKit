#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Gaming.Input.h>


namespace Gaming = winrt::Windows::Gaming::Input;
namespace Foundation = winrt::Windows::Foundation::Collections;

namespace BrokenBytes::ControllerKit::GamingInput {
	auto Init() -> void;
	auto Gamepads()->Foundation::IVectorView<Gaming::Gamepad>;
}
