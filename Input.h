#pragma once

#include"DirectX.h"
#include"windows.h"

#define DIRECTINPUT_VERSION 0x0800		//DirectInputのバージョン指定
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include<wrl.h>

class Input
{
public:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	ComPtr<IDirectInput8> directInput = nullptr;
	ComPtr<IDirectInputDevice8> keyboard;
	BYTE key[256]{};
	BYTE oldkey[256]{};
public:
	void Initialize(HINSTANCE hInstance, HWND hwnd);
	void Update();
	bool IsTrigger(BYTE key_);
	bool IsPress(BYTE key_);
	bool IsRelease(BYTE key_);
};

