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

	//初期化
	void Initialize(WindowsAPI* winApp);
	//更新
	void Update();
	//キーボード押下(瞬間)
	bool IsKeyTrigger(BYTE key_);
	//キーボード押下(押しっぱなし)
	bool IsKeyPress(BYTE key_);
	//キーボード押下(離した瞬間)
	bool IsKeyRelease(BYTE key_);
	//パッド押下(瞬間)
	bool IsPadTrigger(WORD Button);
	//パッド押下(押しっぱなし)
	bool IsPadPress(WORD Button);
	//パッド押下(離した瞬間)
	bool IsPadRelease(WORD Button);
	//Lスティックが左に倒れたか
	bool IsDownLStickLeft(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	//Lスティックが右に倒れたか
	bool IsDownLStickRight(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	//Lスティックが上に倒れたか
	bool IsDownLStickUp(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	//Lスティックが下に倒れたか
	bool IsDownLStickDown(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	//Rスティックが左に倒れたか
	bool IsDownRStickLeft(int deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	//Rスティックが右に倒れたか
	bool IsDownRStickRight(int deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	//Rスティックが上に倒れたか
	bool IsDownRStickUp(int deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	//Rスティックが下に倒れたか
	bool IsDownRStickDown(int deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

	//Rスティックが左右どちらかに倒れたか
	float GetDownRstickX(int deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	//Rスティックが上下どちらかに倒れたか
	float GetDownRstickY(int deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

	//Lスティックが左右どちらかに倒れたか
	float GetDownLstickX(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	//Lスティックが上下どちらかに倒れたか
	float GetDownLstickY(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

};
