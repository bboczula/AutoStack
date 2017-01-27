#pragma once

#include "CBaseWindow.h"

class CWindowsApp : public CBaseWindow
{
public:
	CWindowsApp(HINSTANCE hInst);
	~CWindowsApp();
	void start();
protected:
	LRESULT CALLBACK windowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void virtual initialize() = 0;
	void virtual update() = 0;
	void virtual cleanup() = 0;
};