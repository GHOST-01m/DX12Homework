#include "stdafx.h"
#include "Win32Application.h"

HWND Win32Application::m_hwnd = nullptr;

int Win32Application::Run(DXSample* pSample, HINSTANCE hInstance, int nCmdShow) {
	// Parse the command line parameters
	//解析命令行参数

	int argc;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	pSample->ParseCommandLineArgs(argv, argc);
	LocalFree(argv);//释放指定的本地内存对象并使其句柄无效。

	//初始化窗口类。
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = L"DXSampleClass";
	RegisterClassEx(&windowClass);

	RECT windowRect = { 0, 0, static_cast<LONG>(pSample->GetWidth()), static_cast<LONG>(pSample->GetHeight()) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	//创建窗口并存储句柄。  
	m_hwnd = CreateWindow(
		windowClass.lpszClassName,                  //指定窗口类名
		pSample->GetTitle(),                        //窗口名称
		WS_OVERLAPPEDWINDOW,                        //正在创建的窗口的样式。此参数可以是窗口样式值以及备注部分中指示的控件样式的组合。
		CW_USEDEFAULT,                              //窗口的初始水平位置。
		CW_USEDEFAULT,                              //窗口的初始垂直位置。
		windowRect.right - windowRect.left,           //窗口的宽度（以设备为单位）
		windowRect.bottom - windowRect.top,         //窗口的高度（以设备为单位）
		nullptr,                                    // 正在创建的窗口的父窗口或所有者窗口的句柄。We have no parent window.
		nullptr,                                    // 菜单句柄，或根据窗口样式指定子窗口标识符。We aren't using menus.
		hInstance,                                  //要与窗口关联的模块实例的句柄。
		pSample,                                //指向要通过WM_CREATE消息的lParam参数指向的CREATESTRUCT结构（lpCreateParams成员）传递给窗口的值的指针。此消息在返回之前由该函数发送到创建的窗口。
		);

	// Initialize the sample.  OnInit is defined in each child-implementation of DXSample.
	//初始化样本。 OnInit在DXSample的每个子实现中定义。  
	pSample->OnInit();

	ShowWindow(m_hwnd, nCmdShow);

	// Main sample loop.
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		// Process any messages in the queue.
		// 处理队列中的所有消息。
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	pSample->OnDestroy();

	// Return this part of the WM_QUIT message to Windows.
	//返回WM_QUIT消息的这一部分到Windows。  
	return static_cast<char>(msg.wParam);
}

// Main message handler for the sample.
// 示例的主消息处理程序。
LRESULT CALLBACK Win32Application::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DXSample* pSample = reinterpret_cast<DXSample*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (message)
	{
	case WM_CREATE:
	{
		// Save the DXSample* passed in to CreateWindow.
		LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
	}
	return 0;

	case WM_KEYDOWN:
		if (pSample)
		{
			pSample->OnKeyDown(static_cast<UINT8>(wParam));
		}
		return 0;

	case WM_KEYUP:
		if (pSample)
		{
			pSample->OnKeyUp(static_cast<UINT8>(wParam));
		}
		return 0;

	case WM_PAINT:
		if (pSample)
		{
			pSample->OnUpdate();
			pSample->OnRender();
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	// Handle any messages the switch statement didn't.
	return DefWindowProc(hWnd, message, wParam, lParam);
}
