#include<DirectXTex.h>
#include"DirectX.h"
using namespace DirectX;

class Texture {
public:
	bool isLoadTexture = false;
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	ScratchImage mipChain{};
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	D3D12_RESOURCE_DESC textureResourceDesc{};
	ID3D12Resource* texBuff = nullptr;


public:
	void LoadTexture(const wchar_t texture[]);
	void Initialize(ReDirectX& directX);
	void CreateSRV(ReDirectX& directX, D3D12_CPU_DESCRIPTOR_HANDLE& srvHandle);
};