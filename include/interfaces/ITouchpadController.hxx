#pragma once
#include <vector>

#include "Math.hxx"

namespace BrokenBytes::ControllerKit::Internal {
	class ITouchpadController {
	public:
		virtual ~ITouchpadController() = default;

		/// <summary>
		/// Gets all current touches on the touchpad
		/// </summary>
		/// <returns>The touches represented in 2D coordinates</returns>
		virtual auto GetTouches() -> std::vector<Math::Vector2<float>> = 0;
	};
}
