#include "time.h"

float Time::time = 0;
float Time::deltaTime = 0;
float Time::gameTime = 0;
float Time::gameDeltaTime = 0;
float Time::gameTimeSpeed = 1;

DWORD Time::lastTime = 0;

void Time::Update()
{
	if (lastTime == 0)
		lastTime = timeGetTime() - 1;
	static DWORD old;
	old = lastTime;
	lastTime = timeGetTime();

	deltaTime = (float)(lastTime - old) / 1000.0f;
	gameDeltaTime = deltaTime * gameTimeSpeed;
	time += deltaTime;
	gameTime += gameDeltaTime;
}