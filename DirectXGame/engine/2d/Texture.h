#pragma once
#include<DirectXTex.h>
#include"DirectX.h"
#include<wrl.h>
#include<array>
#include<string>
#include<map>



//テクスチャ構造体を値
struct TextureData {
	//テクスチャリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	//SRVハンドル(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE cpuSRVHandle;
	//SRVハンドル(GPU)
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
 static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;			//デスクリプタヒープ
 static std::map<std::string,TextureData> textures;	//テクスチャバッファ
 static ID3D12Device* dev;



	
public:

	static Texture* GetInstance();

	static TextureData* LoadTexture(std::string filename = "NULL");
	static void Initialize(ID3D12Device* device);

	static ID3D12DescriptorHeap* GetDescHeap() { return descHeap.Get(); }


};

