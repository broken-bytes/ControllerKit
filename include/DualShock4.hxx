#pragma once

#include <string>

#include "Controller.hxx"

namespace BrokenBytes::ControllerKit::Sony
{
	struct Color {
		uint16_t R;
		uint16_t G;
		uint16_t B;
	};

	class DualShock4 : Controller
	{
	public:
		void SetLightBarColor(Color c);
		Color LightBarColor() const;
	private:
		const std::string _vendorId{ "0x054C" };
		const std::string _ids { "0x05C4", "0x09CC" };
	};
}