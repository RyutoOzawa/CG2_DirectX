#pragma once
//入力クラス
//キーボード、パッドの接続や入力を取得するクラス

#include"DirectX.h"
#include"windows.h"
#include"WindowsAPI.h"

#define DIRECTINPUT_VERSION 0x0800		//DirectInputのバージョン指定
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include<wrl.h>
#include<Xinput.h>
#pragma comment(lib,"xinput.lib")

//////////////////////////////////////////
/////////////PADのボタン一覧////////////////
/////////////////////////////////////////
//XINPUT_GAMEPAD_DPAD_UP				スティック上
//XINPUT_GAMEPAD_DPAD_DOWN				スティック下
//XINPUT_GAMEPAD_DPAD_LEFT				スティック左
//XINPUT_GAMEPAD_DPAD_RIGHT				スティック右
//XINPUT_GAMEPAD_START					スタートボタン
//XINPUT_GAMEPAD_BACK					セレクトボタン
//XINPUT_GAMEPAD_LEFT_THUMB				左スティック押し込み
//XINPUT_GAMEPAD_RIGHT_THUMB			右スティック押し込み
//XINPUT_GAMEPAD_LEFT_SHOULDER			Lボタン(LBは別)
//XINPUT_GAMEPAD_RIGHT_SHOULDER			Rボタン(RBは別)
//XINPUT_GAMEPAD_A						Aボタン
//XINPUT_GAMEPAD_B						Bボタン
//XINPUT_GAMEPAD_X						Xボタン
//XINPUT_GAMEPAD_Y						Yボタン


class Input
{
public:

private:
	Microsoft::WRL::ComPtr<IDirectInput8> directInput = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	BYTE key[256]{};
	BYTE oldkey[256]{};

	Input();
	~Input();
public:

	//コピーコンストラクタ無効
	Input(const Input& obj) = delete;
	//代入演算子を無効
	Input& operator=(const Input& obj) = delete;

	//インスタンスアクセス専用関数
	static Input* GetInstance();

	XINPUT_STATE padState;
	XINPUT_STATE oldPadState;

	void Initialize(WindowsAPI* winApp);
	void Update();
	bool IsKeyTrigger(BYTE key_);
	bool IsKeyPress(BYTE key_);
	bool IsKeyRelease(BYTE key_);

	bool IsPadTrigger(WORD Button);
	bool IsPadPress(WORD Button);
	bool IsPadRelease(WORD Button);

	bool IsDownLStickLeft(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	bool IsDownLStickRight(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	bool IsDownLStickUp(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	bool IsDownLStickDown(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	bool IsDownRStickLeft(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	bool IsDownRStickRight(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	bool IsDownRStickUp(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	bool IsDownRStickDown(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
};

