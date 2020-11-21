#include "../include/ControllerKit.hxx"
#include "../include/WorkerThread.hxx"

namespace WorkerThread = BrokenBytes::ControllerKit::Internal::WorkerThread;

namespace BrokenBytes::ControllerKit {
	void Init() {
		WorkerThread::Start();
	}

	void Deinit() {
		WorkerThread::Stop();
	}

	void SetConnectHook(std::function<void(Controller)> callback) {

	}

	void SetDisconnectHookHook(std::function<void(Controller)> callback) {

	}
}