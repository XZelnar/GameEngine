#include "inputManager.h"

#include "../window.h"

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

IDirectInput8* InputManager::d3din = 0;
IDirectInputDevice8* InputManager::keyboard = 0;
IDirectInputDevice8* InputManager::mouse = 0;
IDirectInputDevice8* InputManager::gamepad = 0;

unsigned char* InputManager::kbState = new unsigned char[256];
unsigned char* InputManager::kbStatePrev = new unsigned char[256];
DIMOUSESTATE* InputManager::mState = new DIMOUSESTATE();
DIMOUSESTATE* InputManager::mStatePrev = new DIMOUSESTATE();
long InputManager::mX, InputManager::mY;
DIJOYSTATE2* InputManager::gpState = new DIJOYSTATE2();
DIJOYSTATE2* InputManager::gpStatePrev = new DIJOYSTATE2();

INPUT_TYPE InputManager::curInputType = INPUT_TYPE::KEYBOARD_MOUSE;

HRESULT InputManager::result;

bool InputManager::g_bFilterOutXinputDevices = false;
XINPUT_DEVICE_NODE* InputManager::g_pXInputDeviceList = NULL;

bool InputManager::immovableMouse = false;

void InputManager::Initialize(HINSTANCE hInstance)
{
	mX = Window::GetWidth() / 2;
	mY = Window::GetHeight() / 2;

	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&d3din, NULL);//TODO error handling

	//kb
	result = d3din->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	result = keyboard->SetCooperativeLevel(Window::GetHWND(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);//TODO exclusive, rm bg
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	result = keyboard->Acquire();

	//mouse
	result = d3din->CreateDevice(GUID_SysMouse, &mouse, NULL);
	result = mouse->SetDataFormat(&c_dfDIMouse);
	result = mouse->SetCooperativeLevel(Window::GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);//TODO exclusive
	result = mouse->Acquire();
	
	//gamepad
	IDirectInputJoyConfig8* pJoyCfg = NULL;
	result = d3din->QueryInterface(IID_IDirectInputJoyConfig8, (LPVOID*)&pJoyCfg);
	DIJOYCONFIG prefferedJoyCfg = {0};
	DI_ENUM_CONTEXT enumContext;
    enumContext.pPreferredJoyCfg = &prefferedJoyCfg;
    enumContext.bPreferredJoyCfgValid = false;
	prefferedJoyCfg.dwSize = sizeof(prefferedJoyCfg);
	result = pJoyCfg->GetConfig(0, &prefferedJoyCfg, DIJC_GUIDINSTANCE);
	if (FAILED(result))//no gamepad connected
		return;
    SAFE_RELEASE( pJoyCfg );
	result = d3din->EnumDevices(DI8DEVCLASS_GAMECTRL, GamepadDeviceCallback, &enumContext, DIEDFL_ATTACHEDONLY);
    if( g_bFilterOutXinputDevices )
        CleanupForIsXInputDevice();
	if (!gamepad)
		return;
	
	gamepad->SetDataFormat(&c_dfDIJoystick2);
	gamepad->SetCooperativeLevel(Window::GetHWND(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	gamepad->EnumObjects(GamepadObjectsCallback, (LPVOID)Window::GetHWND(), DIDFT_ALL);
	gamepad->Acquire();
}

void InputManager::Dispose()
{
	if (keyboard)
	{
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = 0;
	}

	if (mouse)
	{
		mouse->Unacquire();
		mouse->Release();
		mouse = 0;
	}

	if (gamepad)
	{
		gamepad->Unacquire();
		gamepad->Release();
		gamepad = 0;
	}

	if (d3din)
	{
		d3din->Release();
		d3din = 0;
	}

	delete[] kbState;
	delete[] kbStatePrev;
	delete mState;
	delete mStatePrev;
}

void InputManager::Update()
{
	updateKeyboard();
	updateMouse();
	updateGamepad();

	//TODO optimize... somehow... ???
	static int tickCounter = 0;
	tickCounter++;
	if (tickCounter == 10)
	{
		tickCounter = 0;
		if (curInputType == INPUT_TYPE::GAMEPAD && (kbIsAnyKeyPressed() || mIsMovedOrAnyButtonPressed()))
			curInputType = INPUT_TYPE::KEYBOARD_MOUSE;
		else if (curInputType == INPUT_TYPE::KEYBOARD_MOUSE && gpIsAnyKeyPressed())
			curInputType = INPUT_TYPE::GAMEPAD;
	}
}

//======================================================================================================================================
//======================================================================================================================================
//======================================================================================================================================

void InputManager::updateKeyboard()
{
	if (!keyboard)
		return;

	static unsigned char* kbBuf;
	kbBuf = kbState;
	kbState = kbStatePrev;
	kbStatePrev = kbBuf;

	result = keyboard->GetDeviceState(sizeof(unsigned char) * 256, (LPVOID)kbState);//TODO proper sizeof???

	if (FAILED(result))
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED || result == E_ACCESSDENIED)
			result = keyboard->Acquire();
}

void InputManager::updateMouse()
{
	if (!mouse)
		return;

	static DIMOUSESTATE* mBuf;
	mBuf = mState;
	mState = mStatePrev;
	mStatePrev = mBuf;

	result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)mState);

	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			mouse->Acquire();
	}
	else
	{
		mX += mState->lX;
		mY += mState->lY;
		if (mX < 0) mX = 0;
		if (mY < 0) mY = 0;
		if (mX > Window::GetWidth()) mX = Window::GetWidth();
		if (mY > Window::GetHeight()) mY = Window::GetHeight();
	}

	if (immovableMouse && GetActiveWindow() == Window::GetHWND())
	{
		static POINT p;
		p.x = 400;
		p.y = 200;
		ClientToScreen(Window::GetHWND(), &p);
		SetCursorPos(p.x, p.y);
	}
}

void InputManager::updateGamepad()
{
	if (!gamepad)
		return;

	static DIJOYSTATE2* gpBuf;
	gpBuf = gpState;
	gpState = gpStatePrev;
	gpStatePrev = gpBuf;

	result = gamepad->Poll();
	if (FAILED(result))
		gamepad->Acquire();
	result = gamepad->GetDeviceState(sizeof(DIJOYSTATE2), gpState);
}

HRESULT InputManager::SetupForIsXInputDevice()
{
    IWbemServices* pIWbemServices = NULL;
    IEnumWbemClassObject* pEnumDevices = NULL;
    IWbemLocator* pIWbemLocator = NULL;
    IWbemClassObject* pDevices[20] = {0};
    BSTR bstrDeviceID = NULL;
    BSTR bstrClassName = NULL;
    BSTR bstrNamespace = NULL;
    DWORD uReturned = 0;
    bool bCleanupCOM = false;
    UINT iDevice = 0;
    VARIANT var;
    HRESULT hr;

    // CoInit if needed
    hr = CoInitialize( NULL );
    bCleanupCOM = SUCCEEDED( hr );

    // Create WMI
    hr = CoCreateInstance( __uuidof( WbemLocator ),
                           NULL,
                           CLSCTX_INPROC_SERVER,
                           __uuidof( IWbemLocator ),
                           ( LPVOID* )&pIWbemLocator );
    if( FAILED( hr ) || pIWbemLocator == NULL )
        goto LCleanup;

    // Create BSTRs for WMI
    bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" ); if( bstrNamespace == NULL ) goto LCleanup;
    bstrDeviceID = SysAllocString( L"DeviceID" );           if( bstrDeviceID == NULL )  goto LCleanup;
    bstrClassName = SysAllocString( L"Win32_PNPEntity" );    if( bstrClassName == NULL ) goto LCleanup;

    // Connect to WMI 
    hr = pIWbemLocator->ConnectServer( bstrNamespace, NULL, NULL, 0L,
                                       0L, NULL, NULL, &pIWbemServices );
    if( FAILED( hr ) || pIWbemServices == NULL )
        goto LCleanup;

    // Switch security level to IMPERSONATE
    CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
                       RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0 );

    // Get list of Win32_PNPEntity devices
    hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, NULL, &pEnumDevices );
    if( FAILED( hr ) || pEnumDevices == NULL )
        goto LCleanup;

    // Loop over all devices
    for(; ; )
    {
        // Get 20 at a time
        hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );
        if( FAILED( hr ) )
            goto LCleanup;
        if( uReturned == 0 )
            break;

        for( iDevice = 0; iDevice < uReturned; iDevice++ )
        {
            // For each device, get its device ID
            hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, NULL, NULL );
            if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL )
            {
                // Check if the device ID contains "IG_".  If it does, then it’s an XInput device
                // Unfortunately this information can not be found by just using DirectInput 
                if( wcsstr( var.bstrVal, L"IG_" ) )
                {
                    // If it does, then get the VID/PID from var.bstrVal
                    DWORD dwPid = 0, dwVid = 0;
                    WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
                    if( strVid && swscanf_s( strVid, L"VID_%4X", &dwVid ) != 1 )
                        dwVid = 0;
                    WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
                    if( strPid && swscanf_s( strPid, L"PID_%4X", &dwPid ) != 1 )
                        dwPid = 0;

                    DWORD dwVidPid = MAKELONG( dwVid, dwPid );

                    // Add the VID/PID to a linked list
                    XINPUT_DEVICE_NODE* pNewNode = new XINPUT_DEVICE_NODE;
                    if( pNewNode )
                    {
                        pNewNode->dwVidPid = dwVidPid;
                        pNewNode->pNext = g_pXInputDeviceList;
                        g_pXInputDeviceList = pNewNode;
                    }
                }
            }
            SAFE_RELEASE( pDevices[iDevice] );
        }
    }

LCleanup:
    if( bstrNamespace )
        SysFreeString( bstrNamespace );
    if( bstrDeviceID )
        SysFreeString( bstrDeviceID );
    if( bstrClassName )
        SysFreeString( bstrClassName );
    for( iDevice = 0; iDevice < 20; iDevice++ )
    SAFE_RELEASE( pDevices[iDevice] );
    SAFE_RELEASE( pEnumDevices );
    SAFE_RELEASE( pIWbemLocator );
    SAFE_RELEASE( pIWbemServices );

    return hr;
}

bool InputManager::IsXInputDevice(const GUID* pGuidProductFromDirectInput)
{
    // Check each xinput device to see if this device's vid/pid matches
    XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
    while( pNode )
    {
        if( pNode->dwVidPid == pGuidProductFromDirectInput->Data1 )
            return true;
        pNode = pNode->pNext;
    }

    return false;
}

BOOL CALLBACK InputManager::GamepadDeviceCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	DI_ENUM_CONTEXT* pEnumContext = ( DI_ENUM_CONTEXT* )pContext;
    HRESULT hr;

    if(g_bFilterOutXinputDevices && IsXInputDevice(&pdidInstance->guidProduct))
        return DIENUM_CONTINUE;

    // Skip anything other than the perferred joystick device as defined by the control panel.  
    // Instead you could store all the enumerated joysticks and let the user pick.
    if( pEnumContext->bPreferredJoyCfgValid &&
        !IsEqualGUID( pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance ) )
        return DIENUM_CONTINUE;

    // Obtain an interface to the enumerated joystick.
    hr = d3din->CreateDevice( pdidInstance->guidInstance, &gamepad, NULL );

    // If it failed, then we can't use this joystick. (Maybe the user unplugged
    // it while we were in the middle of enumerating it.)
    if( FAILED( hr ) )
        return DIENUM_CONTINUE;

    // Stop enumeration. Note: we're just taking the first joystick we get. You
    // could store all the enumerated joysticks and let the user pick.
    return DIENUM_STOP;
}

BOOL CALLBACK InputManager::GamepadObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext)
{
    HWND hDlg = ( HWND )pContext;

    static int nSliderCount = 0;  // Number of returned slider controls
    static int nPOVCount = 0;     // Number of returned POV controls

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg;
        diprg.diph.dwSize = sizeof( DIPROPRANGE );
        diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
        diprg.diph.dwHow = DIPH_BYID;
        diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin = GAMEPAD_AXIS_MIN;
        diprg.lMax = GAMEPAD_AXIS_MAX;

        // Set the range for the axis
        if( FAILED( gamepad->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
            return DIENUM_STOP;

    }


    // Set the UI to reflect what objects the joystick supports
    if( pdidoi->guidType == GUID_XAxis )
    {
        EnableWindow( GetDlgItem( hDlg, IDC_X_AXIS ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_X_AXIS_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_YAxis )
    {
        EnableWindow( GetDlgItem( hDlg, IDC_Y_AXIS ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Y_AXIS_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_ZAxis )
    {
        EnableWindow( GetDlgItem( hDlg, IDC_Z_AXIS ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Z_AXIS_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_RxAxis )
    {
        EnableWindow( GetDlgItem( hDlg, IDC_X_ROT ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_X_ROT_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_RyAxis )
    {
        EnableWindow( GetDlgItem( hDlg, IDC_Y_ROT ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Y_ROT_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_RzAxis )
    {
        EnableWindow( GetDlgItem( hDlg, IDC_Z_ROT ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Z_ROT_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_Slider )
    {
        switch( nSliderCount++ )
        {
            case 0 :
                EnableWindow( GetDlgItem( hDlg, IDC_SLIDER0 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_SLIDER0_TEXT ), TRUE );
                break;

            case 1 :
                EnableWindow( GetDlgItem( hDlg, IDC_SLIDER1 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_SLIDER1_TEXT ), TRUE );
                break;
        }
    }
    if( pdidoi->guidType == GUID_POV )
    {
        switch( nPOVCount++ )
        {
            case 0 :
                EnableWindow( GetDlgItem( hDlg, IDC_POV0 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_POV0_TEXT ), TRUE );
                break;

            case 1 :
                EnableWindow( GetDlgItem( hDlg, IDC_POV1 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_POV1_TEXT ), TRUE );
                break;

            case 2 :
                EnableWindow( GetDlgItem( hDlg, IDC_POV2 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_POV2_TEXT ), TRUE );
                break;

            case 3 :
                EnableWindow( GetDlgItem( hDlg, IDC_POV3 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_POV3_TEXT ), TRUE );
                break;
        }
    }

    return DIENUM_CONTINUE;
}

void InputManager::CleanupForIsXInputDevice()
{
    // Cleanup linked list
    XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
    while( pNode )
    {
        XINPUT_DEVICE_NODE* pDelete = pNode;
        pNode = pNode->pNext;
        SAFE_DELETE( pDelete );
    }
}

//======================================================================================================================================
//======================================================================================================================================
//======================================================================================================================================

bool InputManager::UseGamepad()
{
	return curInputType == INPUT_TYPE::GAMEPAD;
}

//===========================================================================kb
//
bool InputManager::kbIsKeyDown(int keyCode)
{
	return kbState[keyCode] && !kbStatePrev[keyCode];
}

bool InputManager::kbIsKeyPressed(int keyCode)
{
	return kbState[keyCode];
}

bool InputManager::kbIsKeyUp(int keyCode)
{
	return !kbState[keyCode] && kbStatePrev[keyCode];
}

bool InputManager::kbIsAnyKeyPressed()
{
	for (int i = 0; i < 256; i++)
		if (kbState[i])
			return true;
	return false;
}

//===========================================================================mouse
//
bool InputManager::mIsButtonDown(IM_BUTTON button)
{
	return mState->rgbButtons[button] && !mStatePrev->rgbButtons[button];
}

bool InputManager::mIsButtonPressed(IM_BUTTON button)
{
	return mState->rgbButtons[button];
}

bool InputManager::mIsButtonUp(IM_BUTTON button)
{
	return !mState->rgbButtons[button] && mStatePrev->rgbButtons[button];
}

float InputManager::mGetDeltaX()
{
	return mState->lX;
}

float InputManager::mGetDeltaY()
{
	return mState->lY;
}

void InputManager::mGetDelta(long& x, long& y)
{
	x = mState->lX;
	y = mState->lY;
}

void InputManager::mGetPosition(long& x, long& y)
{
	x = mX;
	y = mY;
}

bool InputManager::mIsMovedOrAnyButtonPressed()
{
	return mState->lX || mState->lY || mState->rgbButtons[0] || mState->rgbButtons[1] || mState->rgbButtons[2];
}

//===========================================================================gamepad
//
bool InputManager::gpIsButtonDown(GP_BUTTONS buttonCode)
{
	if (!gamepad) 
		return false;
	return gpState->rgbButtons[buttonCode] && !gpStatePrev->rgbButtons[buttonCode];
}

bool InputManager::gpIsButtonPressed(GP_BUTTONS buttonCode)
{
	if (!gamepad) 
		return false;
	return gpState->rgbButtons[buttonCode];
}

bool InputManager::gpWasButtonPressed(GP_BUTTONS buttonCode)
{
	if (!gamepad) 
		return false;
	return gpStatePrev->rgbButtons[buttonCode];
}

bool InputManager::gpIsButtonUp(GP_BUTTONS buttonCode)
{
	if (!gamepad) 
		return false;
	return !gpState->rgbButtons[buttonCode] && gpStatePrev->rgbButtons[buttonCode];
}

GP_D_PAD InputManager::gpGetDPadState()
{
	if (!gamepad) 
		return GP_D_PAD::NONE;
	return (GP_D_PAD)gpState->rgdwPOV[0];
}

float InputManager::gpGetAxisDelta(GP_AXIS axis)
{
	if (!gamepad) 
		return 0;
	return gpGetAxisState(axis, true) - gpGetAxisState(axis, false);
}

float InputManager::gpGetAxisState(GP_AXIS axis, bool curState)
{
	if (!gamepad) 
		return 0;
	static DIJOYSTATE2* gpas;
	gpas = curState ? gpState : gpStatePrev;
	switch(axis)
	{
	case GP_AXIS::LEFT_X:
		return GAMEPAD_AXIS_NORMALIZE((float)gpas->lX);
	case GP_AXIS::LEFT_Y:
		return GAMEPAD_AXIS_NORMALIZE((float)gpas->lY);
	case GP_AXIS::RIGHT_X:
		return GAMEPAD_AXIS_NORMALIZE((float)gpas->lRx);
	case GP_AXIS::RIGHT_Y:
		return GAMEPAD_AXIS_NORMALIZE((float)gpas->lRy);
	case GP_AXIS::TRGGERS:
		return GAMEPAD_AXIS_NORMALIZE((float)gpas->lZ);
	}
	return 0;
}

bool InputManager::gpIsInputDown(GP_INPUT input)
{
	if (!gamepad) 
		return false;
	return gpGetInputState(input, true) && !gpGetInputState(input, false);
}

bool InputManager::gpIsInputPressed(GP_INPUT input)
{
	if (!gamepad) 
		return false;
	return gpGetInputState(input, true);
}

bool InputManager::gpIsInputUp(GP_INPUT input)
{
	if (!gamepad) 
		return false;
	return !gpGetInputState(input, true) && gpGetInputState(input, false);
}

float InputManager::gpGetInputState(GP_INPUT input, bool curState)
{
	if (!gamepad) 
		return 0;

	static DIJOYSTATE2* gpis;
	gpis = curState ? gpState : gpStatePrev;
	auto isBPressed = [curState](GP_BUTTONS b) { return curState ? gpIsButtonPressed(b) : gpWasButtonPressed(b); };//TODO optimize???
	
	switch(input)
	{
	case GP_INPUT::A:
		return isBPressed(GP_BUTTONS::B_A);
	case GP_INPUT::B:
		return isBPressed(GP_BUTTONS::B_B);
	case GP_INPUT::X:
		return isBPressed(GP_BUTTONS::B_X);
	case GP_INPUT::Y:
		return isBPressed(GP_BUTTONS::B_Y);
	case GP_INPUT::LB:
		return isBPressed(GP_BUTTONS::B_LB);
	case GP_INPUT::RB:
		return isBPressed(GP_BUTTONS::B_RB);
	case GP_INPUT::BACK:
		return isBPressed(GP_BUTTONS::B_BACK);
	case GP_INPUT::START:
		return isBPressed(GP_BUTTONS::B_START);
	case GP_INPUT::LS:
		return isBPressed(GP_BUTTONS::B_LS);
	case GP_INPUT::RS:
		return isBPressed(GP_BUTTONS::B_RS);
	case GP_INPUT::D_PAD_UP:
		return gpis->rgdwPOV[0] == 0 || gpis->rgdwPOV[0] == 4500;
	case GP_INPUT::D_PAD_RIGHT:
		return gpis->rgdwPOV[0] == 9000 || gpis->rgdwPOV[0] == 13500;
	case GP_INPUT::D_PAD_DOWN:
		return gpis->rgdwPOV[0] == 18000 || gpis->rgdwPOV[0] == 21500;
	case GP_INPUT::D_PAD_LEFT:
		return gpis->rgdwPOV[0] == 27000 || gpis->rgdwPOV[0] == 31500;
	case GP_INPUT::AXIS_LEFT_X:
		return gpGetAxisState(GP_AXIS::LEFT_X, curState);
	case GP_INPUT::AXIS_LEFT_Y:
		return gpGetAxisState(GP_AXIS::LEFT_Y, curState);
	case GP_INPUT::AXIS_RIGHT_X:
		return gpGetAxisState(GP_AXIS::RIGHT_X, curState);
	case GP_INPUT::AXIS_RIGHT_Y:
		return gpGetAxisState(GP_AXIS::RIGHT_Y, curState);
	case GP_INPUT::AXIS_TRIGGERS:
		return gpGetAxisState(GP_AXIS::TRGGERS, curState);
	case GP_INPUT::LT:
		return gpGetAxisState(GP_AXIS::TRGGERS, curState) > 0.5f;
	case GP_INPUT::RT:
		return gpGetAxisState(GP_AXIS::TRGGERS, curState) < -0.5f;
	}
}

bool InputManager::gpIsAnyKeyPressed()
{
	if (!gamepad) 
		return false;
	for (int i = 0; i < 21; i++)
		if (gpGetInputState((GP_INPUT)(1 << i)))
			return true;
	return false;
}