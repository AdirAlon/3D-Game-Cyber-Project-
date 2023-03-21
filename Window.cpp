#include "Window.h"
#include <exception>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		break;
	}

	case WM_DESTROY:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}

	case WM_SETFOCUS:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window)
			window->onFocus();
		break;
	}

	case WM_SIZE:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window) window->onSize();
		break;
	}

	case WM_KILLFOCUS:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onKillFocus();
		break;
	}

	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	} 
	return NULL;
}

Window::Window()
{
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"GameWindow";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	if (!::RegisterClassEx(&wc))
		throw std::exception("window creation failed");

	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"GameWindow", L"Game", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, NULL);

	if (!m_hwnd)
		throw std::exception("window creation failed");

	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);


	m_running = true;
}

Window::~Window()
{

}

bool Window::broadcast()
{
	MSG msg;

	if (!this->m_init_flag)
	{
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		this->onCreate();
		this->m_init_flag = true;
	}

	this->onUpdate();

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	

	Sleep(0);

	return true;
}

bool Window::isRunning()
{
	if (m_running)
		broadcast();
	return m_running;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}

RECT Window::getScreenSize()
{
	RECT rc;
	rc.right = ::GetSystemMetrics(SM_CXSCREEN);
	rc.bottom = ::GetSystemMetrics(SM_CYSCREEN);
	return rc;
}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
	m_running = false;
}

void Window::onFocus()
{
}

void Window::onKillFocus()
{
}

void Window::onSize()
{
}
