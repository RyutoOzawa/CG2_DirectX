#pragma once
#include<Windows.h>

class WindowsAPI
{
public:
	const int winW = 1280;
	const int winH = 720;
	WNDCLASSEX w{};//ウィンドウクラス
	RECT wrc{};//ウィンドウサイズ
	HWND hwnd{};//ウィンドウオブジェクト
	MSG msg{};//メッセージ

public:
	WindowsAPI();
	~WindowsAPI();
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void Initialize();

};

