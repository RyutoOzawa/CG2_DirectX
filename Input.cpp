#include "Input.h"
#include<wrl.h>


#include<cassert>
using namespace Microsoft::WRL;

void Input::Initialize(WindowsAPI* windowsApi)
{
	HRESULT result_;
	//借りてきたWinAppのインスタンスを記録
	this->windowsApi = windowsApi;

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
}

bool Input::IsTrigger(BYTE key_)
{
	return (key[key_] && !oldkey[key_]);
}

bool Input::IsPress(BYTE key_)
{
	return key[key_];
}

bool Input::IsRelease(BYTE key_)
{
	return (!key[key_] && oldkey[key_]);
}

//パッド用関数

DWORD Input::Updatekeypad(DWORD dwUserIndex)
{
	oldGamePad = gamePad;

	return XInputGetState(
		dwUserIndex,//複数つながれてるときの選択
		&gamePad);//この変数に入力状況が格納される
}

float Input::PadAnalogStickLX()
{
	return (float)gamePad.Gamepad.sThumbLX / SHRT_MAX;
}

float Input::PadAnalogStickLY()
{
	return (float)gamePad.Gamepad.sThumbLY / SHRT_MAX;
}

float Input::PadAnalogStickRX()
{
	return (float)gamePad.Gamepad.sThumbRX / SHRT_MAX;
}

float Input::PadAnalogStickRY()
{
	return (float)gamePad.Gamepad.sThumbRY / SHRT_MAX;
}

float Input::PadLTrigger()
{
	return (float)gamePad.Gamepad.bLeftTrigger / 255;
}

float Input::PadRTrigger()
{
	return (float)gamePad.Gamepad.bRightTrigger / 255;
}

bool Input::PadKey(int button)
{
	return gamePad.Gamepad.wButtons & button;
}

float Input::OldPadAnalogStickLX()
{
	return (float)oldGamePad.Gamepad.sThumbLX / SHRT_MAX;
}

float Input::OldPadAnalogStickLY()
{
	return (float)oldGamePad.Gamepad.sThumbLY / SHRT_MAX;
}

float Input::OldPadAnalogStickRX()
{
	return (float)oldGamePad.Gamepad.sThumbRX / SHRT_MAX;
}

float Input::OldPadAnalogStickRY()
{
	return (float)oldGamePad.Gamepad.sThumbRY / SHRT_MAX;
}

float Input::OldPadLTrigger()
{
	return (float)oldGamePad.Gamepad.bLeftTrigger / 255;
}

float Input::OldPadRTrigger()
{
	return (float)oldGamePad.Gamepad.bRightTrigger / 255;
}

bool Input::TriggerPadKey(int button)
{

	return ((gamePad.Gamepad.wButtons & button) == button && (oldGamePad.Gamepad.wButtons & button) != button);
}

