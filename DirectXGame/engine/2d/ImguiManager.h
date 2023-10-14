#pragma once
//imguiマネージャ
//デバッグ用のimguiを使うためのクラス

#include<imgui.h>
#include<imgui_impl_win32.h>
#include<imgui_impl_dx12.h>
#include<dxgi1_4.h>
#include"WindowsAPI.h"
#include"DirectX.h"
#include<wrl.h>

class ImguiManager
{
private:
	ImguiManager();
	~ImguiManager();

public:
	//コピーコンストラクタ無効
	ImguiManager(const ImguiManager& obj) = delete;
	//代入演算子を無効
	ImguiManager& operator=(const ImguiManager& obj) = delete;

	//インスタンスアクセス専用関数
	static ImguiManager* GetInstance();

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

	ReDirectX* directX = nullptr;

	//初期化
	void Initialize(WindowsAPI* winApp_, ReDirectX* directX_);

	//コマンド収集開始
	void Begin();

	//コマンド収集終了
	void End();

	//描画
	void Draw();

	//終了
	void Finalize();
};

