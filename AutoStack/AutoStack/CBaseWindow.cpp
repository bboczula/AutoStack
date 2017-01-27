#include "CBaseWindow.h"

CBaseWindow::CBaseWindow(HINSTANCE hInst) : hWnd(NULL), instance(hInst)
{
}

CBaseWindow::~CBaseWindow()
{
}

bool CBaseWindow::registerWindow()
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = CBaseWindow::stWindowProcedure;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "AutoStackMainWindow";

	if (!RegisterClassEx(&wc))
	{
		return false;
	}
	else
	{
		return true;
	}
}

LRESULT CBaseWindow::stWindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CBaseWindow* pWnd;

	if (message == WM_NCCREATE)
	{
		SetWindowLong(hWnd, GWL_USERDATA, (long)((LPCREATESTRUCT(lParam))->lpCreateParams));
	}

	pWnd = getObjectWindow(hWnd);

	if (pWnd)
	{
		return pWnd->windowProcedure(hWnd, message, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

bool CBaseWindow::createWindow(LPCSTR title, LONG width, LONG height)
{
	this->width = width;
	this->height = height;

	hWnd = CreateWindow("AutoStackMainWindow", title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, GetModuleHandle(NULL), (void*)this);

	this->hWnd = hWnd;

	ShowWindow(hWnd, SW_SHOW);

	return (hWnd != NULL);
}

HWND CBaseWindow::getHwnd()
{
	return hWnd;
}

HWND* CBaseWindow::getHwndPtr()
{
	return &hWnd;
}