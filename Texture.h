#include<DirectXTex.h>
#include"DirectX.h"
#include<wrl.h>

class Texture {

public:

	bool isLoadTexture = false;
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};
	DirectX::ScratchImage mipChain{};

	D3D12_RESOURCE_DESC textureResourceDesc{};
	D3D12_GPU_DESCRIPTOR_HANDLE  texAdress{}; //テクスチャのGPUアドレス
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

public:
	void LoadTexture(const wchar_t texture[]);
	void Initialize(Microsoft::WRL::ComPtr<ID3D12Device> device);
	void CreateSRV(Microsoft::WRL::ComPtr<ID3D12Device> device, D3D12_CPU_DESCRIPTOR_HANDLE& srvHandle);
};