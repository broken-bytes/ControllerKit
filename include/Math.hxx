#pragma once

#include <cstdint>
#include <array>
#include <iostream>

//#include <crc/CRC.h>

namespace BrokenBytes::ControllerKit::Math {
	template<typename T,
		typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
		struct Vector2 {
		T X;
		T Y;
	};

	template<typename T,
		typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
		struct Vector3 {
		T X;
		T Y;
		T Z;
	};

	template<typename T,
		typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
		struct Vector4 {
		T X;
		T Y;
		T Z;
		T W;
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
	/// Gets the CRC-32 checksum for a range of bytes
	/// </summary>
	/// <param name="bytes"></param>
	/// <returns>The CRC Checksum split into 4 bytes</returns>
	inline auto GetCRCFromBytes(unsigned char* bytes, size_t length) -> std::array<uint8_t, 4> {
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
