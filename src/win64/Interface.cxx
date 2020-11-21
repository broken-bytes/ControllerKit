#include <Windows.h>
extern "C" {
	#include <hidsdi.h>
}
#include <iostream>

#include "../../include/Interface.hxx"

namespace BrokenBytes::ControllerKit::Internal::Interface {
	void ReadDevices() {
		
		std::cout << "Test" << " 134" << std::endl;
	}

	void QueryDevices() {

	}
}