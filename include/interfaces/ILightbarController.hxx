#pragma once


namespace BrokenBytes::ControllerKit {
	struct Color;
}

namespace BrokenBytes::ControllerKit::Internal {
	class ILightbarController {
	public:
		virtual ~ILightbarController() = default;

		/// <summary>
		/// Sets the color of the lightbar
		/// </summary>
		/// <param name="c">The color in 32 bit RGB unsigned short format</param>
		virtual auto SetLightbarColor(Color c) -> void = 0;
	};
}
