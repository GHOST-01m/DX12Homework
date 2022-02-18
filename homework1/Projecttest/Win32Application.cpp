#include "stdafx.h"
#include "Win32Application.h"

HWND Win32Application::m_hwnd = nullptr;

int Win32Application::Run(DXSample* pSample, HINSTANCE hInstance, int nCmdShow) {
	// Parse the command line parameters
	//���������в���

	int argc;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	pSample->ParseCommandLineArgs(argv, argc);
	LocalFree(argv);//�ͷ�ָ���ı����ڴ����ʹ������Ч��

	//��ʼ�������ࡣ
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

	//�������ڲ��洢�����  
	m_hwnd = CreateWindow(
		windowClass.lpszClassName,                  //ָ����������
		pSample->GetTitle(),                        //��������
		WS_OVERLAPPEDWINDOW,                        //���ڴ����Ĵ��ڵ���ʽ���˲��������Ǵ�����ʽֵ�Լ���ע������ָʾ�Ŀؼ���ʽ����ϡ�
		CW_USEDEFAULT,                              //���ڵĳ�ʼˮƽλ�á�
		CW_USEDEFAULT,                              //���ڵĳ�ʼ��ֱλ�á�
		windowRect.right - windowRect.left,           //���ڵĿ�ȣ����豸Ϊ��λ��
		windowRect.bottom - windowRect.top,         //���ڵĸ߶ȣ����豸Ϊ��λ��
		nullptr,                                    // ���ڴ����Ĵ��ڵĸ����ڻ������ߴ��ڵľ����We have no parent window.
		nullptr,                                    // �˵����������ݴ�����ʽָ���Ӵ��ڱ�ʶ����We aren't using menus.
		hInstance,                                  //Ҫ�봰�ڹ�����ģ��ʵ���ľ����
		pSample,                                //ָ��Ҫͨ��WM_CREATE��Ϣ��lParam����ָ���CREATESTRUCT�ṹ��lpCreateParams��Ա�����ݸ����ڵ�ֵ��ָ�롣����Ϣ�ڷ���֮ǰ�ɸú������͵������Ĵ��ڡ�
		);

	// Initialize the sample.  OnInit is defined in each child-implementation of DXSample.
	//��ʼ�������� OnInit��DXSample��ÿ����ʵ���ж��塣  
	pSample->OnInit();

	ShowWindow(m_hwnd, nCmdShow);

	// Main sample loop.
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		// Process any messages in the queue.
		// ��������е�������Ϣ��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	pSample->OnDestroy();

	// Return this part of the WM_QUIT message to Windows.
	//����WM_QUIT��Ϣ����һ���ֵ�Windows��  
	return static_cast<char>(msg.wParam);
}

// Main message handler for the sample.
// ʾ��������Ϣ�������
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
