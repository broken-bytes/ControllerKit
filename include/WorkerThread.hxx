#pragma once

#include <thread>
#include <cstdint>

#include "Interface.hxx"


constexpr uint_fast8_t sleep = 500;

namespace Interface = BrokenBytes::ControllerKit::Internal::Interface;
namespace BrokenBytes::ControllerKit::Internal::WorkerThread {};
namespace WorkerThread = BrokenBytes::ControllerKit::Internal::WorkerThread;
void Run();

namespace BrokenBytes::ControllerKit::Internal::WorkerThread {
	std::thread _runner;
	bool _active = false;

	inline void Start() {
		_active = true;
		Interface::QueryDevices();
		_runner = std::thread(&Run);
	}

	inline void Stop() {
		_active = false;
	}
}

void Run() {
	while (WorkerThread::_active) {
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(2));
		Interface::ReadDevices();
	}
}

