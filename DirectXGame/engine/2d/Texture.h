#pragma once
#include<DirectXTex.h>
#include"DirectX.h"
#include<wrl.h>
#include<array>
#include<string>
#include<map>



//�e�N�X�`���\���̂�l
struct TextureData {
	//�e�N�X�`�����\�[�X
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	//SRV�n���h��(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE cpuSRVHandle;
	//SRV�n���h��(GPU)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuSRVHandle;

};


class Texture {

public:


private:

	Texture() = default;
	~Texture() = default;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;


 static std::string defaultBaseDirectory ;
 static const size_t spriteSRVCount = 2048;
 static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;			//�f�X�N���v�^�q�[�v
 static std::map<std::string,TextureData> textures;	//�e�N�X�`���o�b�t�@
 static ID3D12Device* dev;



	
public:

	static Texture* GetInstance();

	static TextureData* LoadTexture(std::string filename = "NULL");
	static void Initialize(ID3D12Device* device);

	static ID3D12DescriptorHeap* GetDescHeap() { return descHeap.Get(); }


};

