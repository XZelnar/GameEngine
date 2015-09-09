#pragma once

template<class T>
class Value
{
public:
	typedef T type;
	virtual T GetValue(float state) = 0;

	void __DoNotDelete();
};

template<class T>
class ValueConstant : public Value<T>
{
public:
	ValueConstant(T v) { value = v; }
	virtual T GetValue(float state) { return value; }

public:
	T value;
};

template<class T>
class ValueLinear : public Value<T>
{
public:
	ValueLinear(T _a, T _b) { a = _a; b = _b; }
	virtual T GetValue(float state) { if (state < 0) state = 0; if (state > 1) state = 1; return a + (b - a) * state; }

public:
	T a, b;
};