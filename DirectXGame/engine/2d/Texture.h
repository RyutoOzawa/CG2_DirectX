#pragma once
#include<DirectXTex.h>
#include"DirectX.h"
#include<wrl.h>
#include<array>
#include<string>

class Texture {
public:

	static std::string defaultBaseDirectory;
	static const size_t spriteSRVCount = 2048;
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;			//�f�X�N���v�^�q�[�v
	static std::array< Microsoft::WRL::ComPtr<ID3D12Resource>, spriteSRVCount >texBuffuers;	//�e�N�X�`���o�b�t�@
	static D3D12_RESOURCE_DESC textureResourceDesc;
	static ID3D12Device* dev;


	
public:
	static uint32_t LoadTexture(std::string filename = "NULL");
	static void Initialize(ID3D12Device* device);
	static void CreateSRV(ID3D12Resource* texBuff, D3D12_CPU_DESCRIPTOR_HANDLE& srvHandle);

	//�w��ԍ��̃e�N�X�`���o�b�t�@���擾
	static ID3D12Resource* GetTextureBuffer(uint32_t index) { return texBuffuers[index].Get(); }

};
