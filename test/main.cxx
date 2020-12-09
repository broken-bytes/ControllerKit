#ifndef UNICODE
#define UNICODE
#endif 

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>     /* for _O_TEXT and _O_BINARY */
#include <errno.h>     /* for EINVAL */
#include <sys\stat.h>  /* for _S_IWRITE */
#include <share.h>     /* for _SH_DENYNO */

#include <ControllerKit.hxx>
#include <iostream>
#include <string>
#include <thread>

using namespace BrokenBytes;
using namespace BrokenBytes::ControllerKit::Types;

void RedirectIOToConsole() {

	//Create a console for this application
	AllocConsole();

	// Get STDOUT handle
	HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
	FILE* COutputHandle = _fdopen(SystemOutput, "w");

	// Get STDERR handle
	HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
	int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
	FILE* CErrorHandle = _fdopen(SystemError, "w");

	// Get STDIN handle
	HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
	int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
	FILE* CInputHandle = _fdopen(SystemInput, "r");

	//make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
	std::ios::sync_with_stdio(true);

	// Redirect the CRT standard input, output, and error handles to the console
	freopen_s(&CInputHandle, "CONIN$", "r", stdin);
	freopen_s(&COutputHandle, "CONOUT$", "w", stdout);
	freopen_s(&CErrorHandle, "CONOUT$", "w", stderr);

	//Clear the error state for each of the C++ standard stream objects. We need to do this, as
	//attempts to access the standard streams before they refer to a valid target will cause the
	//iostream objects to enter an error state. In versions of Visual Studio after 2005, this seems
	//to always occur during startup regardless of whether anything has been read from or written to
	//the console or not.
	std::wcout.clear();
	std::cout.clear();
	std::wcerr.clear();
	std::cerr.clear();
	std::wcin.clear();
	std::cin.clear();
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

std::thread t;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Learn to Program Windows",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	RedirectIOToConsole();
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Run the message loop.

	ControllerKit::Init();
	ControllerKit::OnControllerConnected([](
		uint8_t id,
		ControllerKit::Types::ControllerType type
		) {
		});
	ControllerKit::OnControllerDisconnected([](
		uint8_t id
		) {
			std::cout << "Disconnected" << id << std::endl;
		});

	t = std::thread([]() {
		while (true) {
			for (auto item : ControllerKit::Controllers()) {
				auto input = item.GetAxis(ControllerKit::Types::Axis::LeftTrigger);

				if (input > 0.2f) {
					item.SetTriggerAdvanced(
						ControllerKit::Types::Trigger::Left,
						1.0f,
						0.0f,
						0.6f,
						1.0f,
						0.05f,
						false
					);
				}
				else {
					item.SetTriggerDisabled(ControllerKit::Types::Trigger::Left);
				}

				input = item.GetAxis(Axis::RightTrigger);


				item.SetImpulseTrigger(Trigger::Right, input);

				auto state = item.GetButtonState(Button::Cross);
				if (state == ButtonState::Up) {
					item.SetLightbarColor({ 255,0,0 });
				}

				if (state == ButtonState::Pressed) {
					item.SetLightbarColor({ 0,255,0 });
				}

				if (state == ButtonState::Down) {
					item.SetLightbarColor({ 0,0,255 });
				}

				if (state == ButtonState::Released) {
					item.SetLightbarColor({ 0,0,0 });
				}
			}
			ControllerKit::Next();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		}
	);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);



		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}