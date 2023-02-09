#pragma once
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
	//�R�s�[�R���X�g���N�^����
	ImguiManager(const ImguiManager& obj) = delete;
	//������Z�q�𖳌�
	ImguiManager& operator=(const ImguiManager& obj) = delete;

	//�C���X�^���X�A�N�Z�X��p�֐�
	static ImguiManager* GetInstance();

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

	ReDirectX* directX = nullptr;

	void Initialize(WindowsAPI* winApp_, ReDirectX* directX_);

	void Begin();

	void End();

	void Draw();

	void Finalize();
};
