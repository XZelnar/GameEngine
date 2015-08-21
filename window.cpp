#include "window.h"
#include "Graphics/graphicsEngine.h"
#include "Scene/sceneManager.h"

HWND Window::hWnd;
float Window::width = 1280;
float Window::height = 760;
bool Window::isFullscreen = false;

void Window::Initialize(HINSTANCE hInstance, int nCmdShow)
{
	create_window(hInstance, nCmdShow);
	//ShowCursor(false);
}

void Window::GetSize(float& _width, float& _height)
{
	_width = width;
	_height = height;
}

void Window::SetSize(float _width, float _height, bool fullscreen)
{
	if (width == _width && height == _height && isFullscreen == fullscreen)
		return;
	float oldw = width, oldh = height;
	width = _width;
	height = _height;
	isFullscreen = fullscreen;
	SetWindowPos(hWnd, HWND_TOP, 0, 0, (int)width, (int)height, SWP_NOZORDER | SWP_FRAMECHANGED);
	SetFullscreen(fullscreen);

	SceneManager::OnResolutionChanged(width, height, oldw, oldh);
}

bool Window::IsFullscreen()
{
	return isFullscreen;
}

void Window::SetFullscreen(bool fullscreen)
{
	isFullscreen = fullscreen;

	int oldw = width, oldh = height;

	if (fullscreen)
	{
		DWORD style = GetWindowLong(hWnd, GWL_STYLE) & ~((WS_OVERLAPPEDWINDOW | WS_THICKFRAME) & ~WS_MAXIMIZEBOX);
		SetWindowLong(hWnd, GWL_STYLE, style);
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, (int)width, (int)height, SWP_NOZORDER | SWP_FRAMECHANGED);
	}
	else
	{
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, (int)width, (int)height, SWP_FRAMECHANGED);
		DWORD style = GetWindowLong(hWnd, GWL_STYLE) | ((WS_OVERLAPPEDWINDOW | WS_THICKFRAME) & ~WS_MAXIMIZEBOX);
		SetWindowLong(hWnd, GWL_STYLE, style);
	}
}

//======================================================================================================================================
//======================================================================================================================================
//======================================================================================================================================

void Window::create_window(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"GameWindow";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, L"GameWindow", L"_", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 300, 200, width, height, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
}

void Window::updateWindow()
{
	SetWindowPos(hWnd, HWND_TOP, 0, 0, (int)width, (int)height, SWP_NOZORDER | SWP_FRAMECHANGED);
	SetFullscreen(isFullscreen);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool firstSize = true;
	switch(message)
	{
	case WM_SIZE:
		if (firstSize)
			firstSize = false;
		else
			GraphicsEngine::OnWM_SIZE();
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}