#pragma once

#include "Math.hxx"

namespace BrokenBytes::ControllerKit::Internal {
	class IGyroscopeController {
	public:
		virtual ~IGyroscopeController() = default;

		/// <summary>
		/// Reads the gyro data from the controller
		/// </summary>
		/// <returns>A Vector in 8bit float 3D coordinates</returns>
		virtual Math::Vector3<float> ReadGyroscope() = 0;

		/// <summary>
		/// Reads the acceleration data from the controller
		/// </summary>
		/// <returns>A Vector in 8bit float 3D coordinates</returns>
		virtual Math::Vector3<float> ReadAcceleration() = 0;
	};
}
