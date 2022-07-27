#include<DirectXTex.h>
#include"DirectX.h"
#include<wrl.h>

class Texture {
public:

	bool isLoadTexture = false;
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};
	DirectX::ScratchImage mipChain{};
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	D3D12_RESOURCE_DESC textureResourceDesc{};
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;


public:
	void LoadTexture(const wchar_t texture[]);
	void Initialize(ReDirectX& directX);
	void CreateSRV(ReDirectX& directX, D3D12_CPU_DESCRIPTOR_HANDLE& srvHandle);
};