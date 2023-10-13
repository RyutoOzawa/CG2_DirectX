//入力クラス
//キーボード、パッドの接続や入力を取得するクラス

#include "Input.h"
#include<wrl.h>
#include<cassert>
using namespace Microsoft::WRL;
#include"ImguiManager.h"

Input::Input()
{
}

Input::~Input()
{
}

Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}

void Input::Initialize(WindowsAPI* winApp)
{
	HRESULT result_;
	//WinAppのインスタンスを取得
	WindowsAPI* windowsApi = winApp;


	//DirectInputのインスタンス生成
	result_ = DirectInput8Create(
		windowsApi->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result_));
	//キーボードデバイス生成
	result_ = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result_));
	//入力データ形式のセット
	result_ = keyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result_));
	//排他制御レベルのセット
	result_ = keyboard->SetCooperativeLevel(
		windowsApi->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result_));
}

void Input::Update()
{
	//キーボード情報の取得開始
	keyboard->Acquire();
	//前フレームのキー情報を保存
	for (int i = 0; i < 256; i++) {
		oldkey[i] = key[i];
	}
	//全キーの入力状態を取得する
	keyboard->GetDeviceState(sizeof(key), key);

	//パッドの接続確認
	oldPadState = padState;
	DWORD flag;
	flag = XInputGetState(0, &padState);


}

bool Input::IsKeyTrigger(BYTE key_)
{
	return (key[key_] && !oldkey[key_]);
}

bool Input::IsKeyPress(BYTE key_)
{
	return key[key_];
}

bool Input::IsKeyRelease(BYTE key_)
{
	return (!key[key_] && oldkey[key_]);
}

bool Input::IsPadTrigger(WORD Button)
{
	return (padState.Gamepad.wButtons & Button) && ((oldPadState.Gamepad.wButtons & Button) != Button);
}

bool Input::IsPadPress(WORD Button)
{
	return padState.Gamepad.wButtons & Button;
}

bool Input::IsPadRelease(WORD Button)
{
	return (oldPadState.Gamepad.wButtons & Button) && ((padState.Gamepad.wButtons & Button) != Button);
}

bool Input::IsDownLStickLeft(int deadZone)
{
	return padState.Gamepad.sThumbLX < -deadZone;
}

bool Input::IsDownLStickRight(int deadZone)
{
	return padState.Gamepad.sThumbLX > deadZone;
}

bool Input::IsDownLStickUp(int deadZone)
{
	return padState.Gamepad.sThumbLY > deadZone;
}

bool Input::IsDownLStickDown(int deadZone)
{
	return padState.Gamepad.sThumbLY < -deadZone;
}

bool Input::IsDownRStickLeft(int deadZone)
{
	return padState.Gamepad.sThumbRX < -deadZone;
}

bool Input::IsDownRStickRight(int deadZone)
{
	return padState.Gamepad.sThumbRX > deadZone;
}

bool Input::IsDownRStickUp(int deadZone)
{
	return padState.Gamepad.sThumbRY > deadZone;
}

bool Input::IsDownRStickDown(int deadZone)
{
	return padState.Gamepad.sThumbRY < -deadZone;
}
