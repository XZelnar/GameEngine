#pragma once

#include "stdafx.h"

class Window
{
	friend class GraphicsEngine;
	friend int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

public:
	static inline HWND GetHWND() { return hWnd; }
	static inline float GetWidth() { return width; }
	static inline float GetHeight() { return height; }
	static void GetSize(float& _width, float& _height);
	static void SetSize(float _width, float _height, bool fullscreen = false);
	static bool IsFullscreen();

private:
	static void Initialize(HINSTANCE hInstance, int nCmdShow);
	static void create_window(HINSTANCE hInstance, int nCmdShow);
	static void SetFullscreen(bool fullscreen);
	static void updateWindow();

private:
	static HWND hWnd;
	static float width, height;
	static bool isFullscreen;
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);