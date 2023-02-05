#pragma once
#include <Windows.h>
class Window
{
public:
	Window();
	~Window();
private:
	bool broadcast();
public:
	bool isRunning();

	RECT getClientWindowRect();

	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

protected:
	HWND m_hwnd;
	bool m_running;
	bool m_init_flag = false;
	
};

