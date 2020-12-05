#ifndef UNICODE
#define UNICODE
#endif 

#include <Windows.h>

#include <ControllerKit.hxx>
#include <iostream>
#include <thread>

using namespace BrokenBytes;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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

    std::cout << "Test" << std::endl;
    // Run the message loop.

    ControllerKit::Init();
    ControllerKit::OnControllerConnected([](uint8_t id, ControllerKit::Types::ControllerType type) {
        std::cout << id << std::endl;
        });
    ControllerKit::OnControllerConnected([](
        uint8_t id,
        ControllerKit::Types::ControllerType type
        ) {
            std::cout << "" << std::endl;
            Beep(1000, 1);
        });
    ControllerKit::OnControllerDisconnected([](
        uint8_t id
        ) {
            std::cout << "" << std::endl;
            Beep(1000, 1);
        });

    auto controllers = ControllerKit::Controllers();
	for(auto item: controllers) {
        std::cout << item.GetAxis(ControllerKit::Types::Axis::LeftTrigger) << std::endl;
	}
	
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