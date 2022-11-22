#pragma once

#include"DirectX.h"
#include"windows.h"
#include"WindowsAPI.h"
#include<Xinput.h>

#define DIRECTINPUT_VERSION 0x0800		//DirectInputのバージョン指定
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include<wrl.h>
#pragma comment (lib, "xinput.lib")

class Input
{
public:

private://キーボード用メンバ変数
	Microsoft::WRL::ComPtr<IDirectInput8> directInput = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	BYTE key[256]{};
	BYTE oldkey[256]{};

	WindowsAPI* windowsApi = nullptr;
public://キーボード用メンバ関数
	void Initialize(WindowsAPI* windowsApi);
	void Update();
	bool IsTrigger(BYTE key_);
	bool IsPress(BYTE key_);
	bool IsRelease(BYTE key_);
private://パッド用メンバ変数
	XINPUT_STATE gamePad;
	XINPUT_STATE oldGamePad;

public://パッド用メンバ関数
	/// <summary>
	/// コントローラーの状況の取得（毎フレームする）
	/// </summary>
	///<param name="dwUserIndex">つながれてるコントローラーが複数の場合の指定（一つなら0でOK）</param>
	/// <returns>成功か？</returns>
	DWORD Updatekeypad(DWORD dwUserIndex);

	/// <summary>
	/// 左アナログスティックのX方向の入力状況
	/// </summary>
	/// <returns>左アナログスティックのX方向の入力状況を返す(-1～+1)</returns>
	float PadAnalogStickLX();

	/// <summary>
	/// 左アナログスティックのY方向の入力状況
	/// </summary>
	/// <returns>左アナログスティックのY方向の入力状況を返す(-1～+1)</returns>
	float PadAnalogStickLY();

	/// <summary>
	/// 右アナログスティックのX方向の入力状況
	/// </summary>
	/// <returns>右アナログスティックのX方向の入力状況を返す(-1～+1)</returns>
	float PadAnalogStickRX();

	/// <summary>
	/// 右アナログスティックのY方向の入力状況
	/// </summary>
	/// <returns>右アナログスティックのY方向の入力状況を返す(-1～+1)</returns>
	float PadAnalogStickRY();

	/// <summary>
	/// 左トリガー(LZとか呼ばれるやつ)の入力状況
	/// </summary>
	/// <returns>左トリガーの入力状況を返す(0～+1)</returns>
	float PadLTrigger();

	/// <summary>
	/// 右トリガー(RZとか呼ばれるやつ)の入力状況
	/// </summary>
	/// <returns>左トリガーの入力状況を返す(0～+1)</returns>
	float PadRTrigger();

	/// <summary>
	/// コントローラーのボタンの入力状況
	/// </summary>
	/// ///<param name="button">ボタンの種類(XINPUT_GAMEPAD_で始まる定数で指定)</param>
	/// <returns>押されてるか？</returns>
	bool PadKey(int button);

	/// <summary>
	/// 一フレーム前の左アナログスティックのX方向の入力状況
	/// </summary>
	/// <returns>一フレーム前の左アナログスティックのX方向の入力状況を返す(-1～+1)</returns>
	float OldPadAnalogStickLX();

	/// <summary>
	/// 一フレーム前の左アナログスティックのY方向の入力状況
	/// </summary>
	/// <returns>一フレーム前の左アナログスティックのY方向の入力状況を返す(-1～+1)</returns>
	float OldPadAnalogStickLY();

	/// <summary>
	/// 一フレーム前の右アナログスティックのX方向の入力状況
	/// </summary>
	/// <returns>一フレーム前の右アナログスティックのX方向の入力状況を返す(-1～+1)</returns>
	float OldPadAnalogStickRX();

	/// <summary>
	/// 一フレーム前の右アナログスティックのY方向の入力状況
	/// </summary>
	/// <returns>一フレーム前の右アナログスティックのY方向の入力状況を返す(-1～+1)</returns>
	float OldPadAnalogStickRY();

	/// <summary>
	/// 一フレーム前の左トリガー(LZとか呼ばれるやつ)の入力状況
	/// </summary>
	/// <returns>一フレーム前の左トリガーの入力状況を返す(0～+1)</returns>
	float OldPadLTrigger();

	/// <summary>
	/// 一フレーム前の右トリガー(RZとか呼ばれるやつ)の入力状況
	/// </summary>
	/// <returns>一フレーム前の左トリガーの入力状況を返す(0～+1)</returns>
	float OldPadRTrigger();

	/// <summary>
	/// 一フレーム前のコントローラーのボタンの入力状況
	/// </summary>
	/// ///<param name="button">ボタンの種類(XINPUT_GAMEPAD_で始まる定数で指定)</param>
	/// <returns>一フレーム前に押されてるか？</returns>
	bool TriggerPadKey(int button);
};

