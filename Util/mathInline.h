#pragma once

inline float sign(float a)
{
	return a > 0 ? 1.0f : a < 0 ? -1.0f : 0.0f;
}

inline int sign(int a)
{
	return a > 0 ? 1.0f : a < 0 ? -1.0f : 0.0f;
}