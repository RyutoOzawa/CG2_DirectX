#pragma once

#include"DirectX.h"
#include"windows.h"
#include<wrl.h>
#define DIRECTINPUT_VERSION 0x0800		//DirectInputのバージョン指定
#include<dinput.h>

class Input
{
public:
	
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	unsigned char key[256]{};
	unsigned char oldkey[256]{};
public:
	void Initialize(HINSTANCE hInstance,HWND hwnd);
	void Update();
	bool IsTrigger(unsigned char key_);
	bool IsPress(unsigned char key_);
	bool IsRelease(unsigned char key_);
};

