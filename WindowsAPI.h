#pragma once
#include<Windows.h>

class WindowsAPI
{
public:
	const int winW = 1280;
	const int winH = 720;
	WNDCLASSEX w{};//�E�B���h�E�N���X
	RECT wrc{};//�E�B���h�E�T�C�Y
	HWND hwnd{};//�E�B���h�E�I�u�W�F�N�g
	MSG msg{};//���b�Z�[�W

public:
	WindowsAPI();
	~WindowsAPI();
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void Initialize();

};

