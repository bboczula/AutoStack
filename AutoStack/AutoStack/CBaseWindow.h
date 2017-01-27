#pragma once

#include <Windows.h>

class CBaseWindow
{
public:
	CBaseWindow(HINSTANCE hInst);
	~CBaseWindow();
	virtual bool registerWindow();
	virtual bool createWindow(LPCSTR title, LONG width, LONG height);
	static LRESULT CALLBACK stWindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND getHwnd();
	HWND* getHwndPtr();
protected:
	LONG width;
	LONG height;
	HINSTANCE instance;
	HWND hWnd;
	virtual LRESULT CALLBACK windowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	inline static CBaseWindow* getObjectWindow(HWND hWnd)
	{
		return (CBaseWindow*)GetWindowLong(hWnd, GWL_USERDATA);
	}
};