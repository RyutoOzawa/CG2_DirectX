#pragma once
#define DIRECTINPUT_VERSION 0x0800		//DirectInputのバージョン指定
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include"DirectX.h"
#include"WindowsAPI.h"

class Input
{
public:
	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;

	char key[256]{};
	char oldkey[256]{};
public:
	void Initialize(HRESULT result_,WindowsAPI windowsAPI_);
	void Update();
	bool IsTrigger(char key_);
	bool IsPress(char key_);
	bool IsRelease(char key_);
};

