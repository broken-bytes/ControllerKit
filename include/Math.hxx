#pragma once

#include <cstdint>
#include <array>
#include <iostream>
#include <type_traits>

//#include <crc/CRC.h>

namespace BrokenBytes::ControllerKit::Math {
	struct IVector {};

	template<typename T,
		typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		struct Vector2 : public IVector {
		T X;
		T Y;

		Vector2(T X, T Y) {
			this->X = X;
			this->Y = Y;
		}

		auto operator==(const Vector2& rhs) -> bool {
			return this->X == rhs.X && this->Y = rhs.Y;
		}

		auto operator!=(const Vector2& rhs) -> bool {
			return !(this == rhs);
		}

		auto Magnitude() -> T {
			return std::sqrt((X * X) + (Y * Y));
		}
	};

	template<typename T,
		typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		struct Vector3 : IVector {
		T X;
		T Y;
		T Z;

		Vector3(T X, T Y, T Z) {
			this->X = X;
			this->Y = Y;
			this->Z = Z;
		}

		auto operator==(const Vector3& rhs) -> bool {
			return this->X == rhs.X && this->Y = rhs.Y && this->Z == rhs.Z;
		}

		auto operator!=(const Vector3& rhs) -> bool {
			return !(this == rhs);
		}

		auto Magnitude() -> T {
			return std::sqrt((X * X) + (Y * Y) + (Z * Z));
		}
	};

	template<typename T,
		typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		struct Vector4 : IVector {
		T X;
		T Y;
		T Z;
		T W;

		Vector4(T X, T Y, T Z, T W) {
			this->X = X; 
			this->Y = Y; 
			this->Z = Z; 
			this->W = W;
		}

		auto operator==(const Vector4& rhs) -> bool {
			return this->X == rhs.X && this->Y = rhs.Y && this->Z == rhs.Z && this->W == rhs.W;
		}

		auto operator!=(const Vector4& rhs) -> bool {
			return !(this == rhs);
		}

		auto Magnitude() -> T {
			return std::sqrt((X * X) + (Y * Y) + (Z * Z) + (W * W));
		}
	};

	/// <summary>
	/// Converts a value from uint range to float range from (-1) - 1
	/// </summary>
	/// <param name="value"></param>
	/// <returns>The float value</returns>
	inline auto ConvertToSignedFloat(uint8_t value) -> float {
		auto f = static_cast<float>(value);
		f /= 255;
		return static_cast<float>((f * 2) - 1.0f);
	}

	/// <summary>
	/// Converts a value from uint range to float range from 0 - 1
	/// </summary>
	/// <param name="value"></param>
	/// <returns>The float value</returns>
	inline auto ConvertToUnsignedFloat(uint8_t value) -> float {
		auto f = static_cast<float>(value);
		f /= 255;
		return static_cast<float>(f);
	}

	/// <summary>
	/// Converts a value from float range to ushort range from 0 - 255
	/// </summary>
	/// <param name="value"></param>
	/// <returns>The float value</returns>
	inline auto ConvertToUnsignedShort(float value) -> uint8_t {
		auto val = value;
		if (val > 1) {
			val = 1;
		}
		if (val < 0) {
			val = 0;
		}
		val *= 255;
		auto ui = static_cast<uint8_t>(val);
		return ui;
	}

	/// <summary>
	/// Converts a value from double range to ushort range from 0 - 255
	/// </summary>
	/// <param name="value"></param>
	/// <returns>The float value</returns>
	inline auto ConvertToUnsignedShort(double value) -> uint8_t {
		auto val = value;
		if (val > 1) {
			val = 1;
		}
		if (val < 0) {
			val = 0;
		}
		val *= 255;
		auto ui = static_cast<uint8_t>(val);
		return ui;
	}

	/// <summary>
	/// Checks if two numbers are max limit apart
	/// </summary>
	/// <param name="target">Target</param>
	/// <param name="value">The value to check against target</param>
	/// <param name="limit">The max difference</param>
	/// <returns></returns>
	template <
		typename T,
		typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
	>
	auto IsInLimits(T target, T value, T limit) -> bool {
		auto val = target - value;

		if (val < limit || val > -limit) {
			return true;
		}
		return false;
	}

	/// <summary>
	/// Checks if two vectors are max limit apart
	/// </summary>
	/// <param name="target">Target</param>
	/// <param name="value">The value to check against target</param>
	/// <param name="limit">The max difference</param>
	/// <returns></returns>
	template <
		typename T,
		typename V,
		typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type,
		typename = std::enable_if_t<std::is_base_of_v<IVector, V>>
	>
		auto IsInLimits(V target, V value, T limit) -> bool {
		auto val = target.Magnitude() - value.Magnitude();
		if (val < limit || val > -limit) {
			return true;
		}
		return false;
	}

	/// <summary>
	/// Gets the CRC-32 checksum for a range of bytes
	/// </summary>
	/// <param name="bytes"></param>
	/// <returns>The CRC Checksum split into 4 bytes</returns>
	inline auto GetCRCFromBytes(unsigned char* bytes, size_t length)->std::array<uint8_t, 4> {
		//std::uint32_t crc = CRC::Calculate(bytes, length, CRC::CRC_32());
		uint32_t crc = 0;
		auto arr = std::array<uint8_t, 4> {
			static_cast<unsigned char>(crc),
				static_cast<unsigned char>(crc >> 8),
				static_cast<unsigned char>(crc >> 16),
				static_cast<unsigned char>(crc >> 24),
		};

		return arr;
	}
}
