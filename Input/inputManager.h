#pragma once

#include "../stdafx.h"
#include "dinput.h"
#include "gamepadDefines.h"
#include <wbemidl.h>

#define DIRECTINPUT_VERSION 0x0800

enum IM_BUTTON
{
	Left = 0,
	Right = 1,
	Middle = 2
};

enum GP_AXIS
{
	LEFT_X = 0,
	LEFT_Y = 1,
	RIGHT_X = 2,
	RIGHT_Y = 3,
	TRGGERS = 4
};

enum GP_BUTTONS
{
	B_A = 0,
	B_B = 1,
	B_X = 2,
	B_Y = 3,
	B_LB = 4,
	B_RB = 5,
	B_BACK = 6,
	B_START = 7,
	B_LS = 8,
	B_RS = 9
};

enum GP_D_PAD : long long
{
	NONE = 4294967295,
	UP = 0,
	RIGHT_UP = 4500,
	UP_RIGHT = 4500,
	RIGHT = 9000,
	RIGHT_DOWN = 13500,
	DOWN_RIGHT = 13500,
	DOWN = 18000,
	DOWN_LEFT = 22500,
	LEFT_DOWN = 22500,
	LEFT = 27000,
	LEFT_UP = 31500,
	UP_LEFT = 31500
};

enum GP_INPUT
{
	A =				1,
	B =				1 << 1,
	X =				1 << 2,
	Y =				1 << 3,
	LB =			1 << 4,
	RB =			1 << 5,
	BACK =			1 << 6,
	START =			1 << 7,
	LS =			1 << 8,
	RS =			1 << 9,
	D_PAD_UP =		1 << 10,
	D_PAD_RIGHT =	1 << 11,
	D_PAD_DOWN =	1 << 12,
	D_PAD_LEFT =	1 << 13,
	AXIS_LEFT_X =	1 << 14,
	AXIS_LEFT_Y =	1 << 15,
	AXIS_RIGHT_X =	1 << 16,
	AXIS_RIGHT_Y =	1 << 17,
	AXIS_TRIGGERS = 1 << 18,
	LT =			1 << 19,
	RT =			1 << 20
};

enum INPUT_TYPE
{
	KEYBOARD_MOUSE,
	GAMEPAD
};

struct XINPUT_DEVICE_NODE
{
    DWORD dwVidPid;
    XINPUT_DEVICE_NODE* pNext;
};

struct DI_ENUM_CONTEXT
{
    DIJOYCONFIG* pPreferredJoyCfg;
    bool bPreferredJoyCfgValid;
};

class InputManager
{
public:
	static void Initialize(HINSTANCE hInstance);
	static void Dispose();
	static void Update();

private:
	static void updateKeyboard();
	static void updateMouse();
	static void updateGamepad();
	static HRESULT SetupForIsXInputDevice();
	static BOOL CALLBACK GamepadDeviceCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
	static BOOL CALLBACK GamepadObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
	static bool IsXInputDevice(const GUID* pGuidProductFromDirectInput);
	static void InputManager::CleanupForIsXInputDevice();

public:
	static bool UseGamepad();
	//kb
	static bool kbIsKeyDown(int keyCode);
	static bool kbIsKeyPressed(int keyCode);
	static bool kbIsKeyUp(int keyCode);
	static bool kbIsAnyKeyPressed();
	//mouse
	static bool mIsButtonDown(IM_BUTTON button);
	static bool mIsButtonPressed(IM_BUTTON button);
	static bool mIsButtonUp(IM_BUTTON button);
	static void mGetDelta(long& x, long& y);
	static float mGetDeltaX();
	static float mGetDeltaY();
	static inline D3DXVECTOR2 mGetPosition() { return D3DXVECTOR2(mX, mY); }
	static void mGetPosition(long& x, long& y);
	static bool mIsMovedOrAnyButtonPressed();
	//gp
	static bool gpIsButtonDown(GP_BUTTONS buttonCode);
	static bool gpWasButtonPressed(GP_BUTTONS buttonCode);
	static bool gpIsButtonPressed(GP_BUTTONS buttonCode);
	static bool gpIsButtonUp(GP_BUTTONS buttonCode);
	static GP_D_PAD gpGetDPadState();
	static float gpGetAxisDelta(GP_AXIS axis);
	static float gpGetAxisState(GP_AXIS axis, bool curState = true);
	static bool gpIsInputDown(GP_INPUT input);
	static bool gpIsInputPressed(GP_INPUT input);
	static bool gpIsInputUp(GP_INPUT input);
	static float gpGetInputState(GP_INPUT input, bool curState = true);
	static bool gpIsAnyKeyPressed();

public:
	static bool immovableMouse;

private:
	static IDirectInput8* d3din;
	static IDirectInputDevice8* keyboard;
	static IDirectInputDevice8* mouse;
	static IDirectInputDevice8* gamepad;
	
	static unsigned char* kbState;
	static unsigned char* kbStatePrev;
	static DIMOUSESTATE* mState;
	static DIMOUSESTATE* mStatePrev;
	static long mX, mY;
	static DIJOYSTATE2* gpState;
	static DIJOYSTATE2* gpStatePrev;
	
	static INPUT_TYPE curInputType;

	static bool g_bFilterOutXinputDevices;
	static XINPUT_DEVICE_NODE* g_pXInputDeviceList;
	static HRESULT result;
};