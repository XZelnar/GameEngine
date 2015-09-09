#pragma once

#include "../stdafx.h"

class Time
{
	friend int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

public:
	inline static float GetTimeSinceStart() { return time; }
	inline static float GetDeltaTime() { return deltaTime; }
	inline static float GetGameTimeSinceStart() { return gameTime; }
	inline static float GetGameDeltaTime() { return gameDeltaTime; }
	inline static float GetGameTimeSpeed() { return gameTimeSpeed; }

	inline static void SetGameTimeSpeed(float t) { if (t < 0) t = 0; if (t > 100) t = 100; gameTimeSpeed = t; }

private:
	static void Update();
	static void OnLevelChanged();

private:
	static float time;
	static float deltaTime;
	static float gameTime;
	static float gameDeltaTime;
	static float gameTimeSpeed;

	static DWORD lastTime;
};