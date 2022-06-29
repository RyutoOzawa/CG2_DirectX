#include "Texture.h"
#include<string.h>
#include<cassert>


void Texture::LoadTexture(const wchar_t texture[])
{
	HRESULT result;
	result = LoadFromWICFile(
		texture,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (!isLoadTexture)isLoadTexture = true;
}

void Texture::Initialize(ReDirectX& directX)
{
	if (isLoadTexture) {
		HRESULT result;
		//ミップマップ生成
		result = GenerateMipMaps(
			scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
			TEX_FILTER_DEFAULT, 0, mipChain);
		if (SUCCEEDED(result)) {
			scratchImg = std::move(mipChain);
			metadata = scratchImg.GetMetadata();
		}

		// 読み込んだディフューズテクスチャをSRGBとして扱う
		metadata.format = MakeSRGB(metadata.format);

		//ヒープ設定
		textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
		textureHeapProp.CPUPageProperty =
			D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		//リソース設定
		textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureResourceDesc.Format = metadata.format;
		textureResourceDesc.Width = metadata.width;	// 幅
		textureResourceDesc.Height = (UINT)metadata.height;	// 高さ
		textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
		textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
		textureResourceDesc.SampleDesc.Count = 1;

		result = directX.device->CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&texBuff));

		//全ミップマップについて
		for (size_t i = 0; i < metadata.mipLevels; i++) {
			//ミップマップレベルを指定してイメージを取得
			const Image* img = scratchImg.GetImage(i, 0, 0);
			//テクスチャバッファにデータ転送
			result = texBuff->WriteToSubresource(
				(UINT)i,
				nullptr,				//全領域へコピー
				img->pixels,			//元データアドレス
				(UINT)img->rowPitch,	//1ラインサイズ
				(UINT)img->slicePitch	//全サイズ
			);
			assert(SUCCEEDED(result));
		}

	}
}

void Texture::CreateSRV(ReDirectX& directX, D3D12_CPU_DESCRIPTOR_HANDLE& srvHandle)
{

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	directX.device->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);
}
