#pragma once

#include"DirectX.h"
#include"windows.h"

class Input
{
public:
	

	unsigned char key[256]{};
	unsigned char oldkey[256]{};
public:
	void Initialize(HINSTANCE hInstance,HWND hwnd);
	void Update();
	bool IsTrigger(unsigned char key_);
	bool IsPress(unsigned char key_);
	bool IsRelease(unsigned char key_);
};

