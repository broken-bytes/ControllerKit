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

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Run the message loop.

	ControllerKit::Init();
	ControllerKit::OnControllerConnected([](
		uint8_t id,
		ControllerKitControllerType type
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
				auto input = item.GetAxis(RightTrigger);
				if (input > 0.8f) {
					item.SetTriggerAdvanced(
						TriggerRight,
						1,
						0,
						0,
						1.0f,
						0.04f,
						false
					);
				}
				else {
					item.SetTriggerSectional(TriggerRight, 0.3f, 0.6f, 1);
					
				}

				item.SetTriggerSectional(TriggerLeft, 0.3f, 0.6f, 1);
				input = item.GetAxis(RightTrigger);


				item.SetImpulseTrigger(RumbleTriggerRight, input / 100);

				auto state = item.GetButtonState(Cross);
				if (state == ButtonUp) {
					item.SetLightbarColor({ 255,0,0 });
				}

				if (state == ButtonPressed) {
					item.SetLightbarColor({ 0,255,0 });
				}

				if (state == ButtonDown) {
					item.SetLightbarColor({ 0,0,255 });
				}

				if (state == ButtonReleased) {
					item.SetLightbarColor({ 255,0,255 });
				}
			}
			ControllerKit::Next();
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