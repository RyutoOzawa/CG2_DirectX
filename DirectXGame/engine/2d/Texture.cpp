#include "Texture.h"
#include<string.h>
#include<cassert>
#include"Util.h"
#include<utility>

using namespace DirectX;
using namespace Microsoft::WRL;


std::string Texture::defaultBaseDirectory = "Resources/";
ComPtr<ID3D12DescriptorHeap>Texture::descHeap;			//デスクリプタヒープ
std::map< std::string, TextureData > Texture::textures;	//テクスチャバッファ
ID3D12Device* Texture::dev = nullptr;

Texture* Texture::GetInstance()
{
	static Texture instance;

	return &instance;
}

TextureData* Texture::LoadTexture(std::string filename)
{

	std::string fullpath;


	//既にリソースファイルまでディレクトリを上っているならフルパス結合はしない
	if (filename.find(defaultBaseDirectory) != SIZE_MAX) {
		fullpath = filename;
	}
	else {
		fullpath = defaultBaseDirectory + filename;
	}

	//ユニコード文字列に変換する
	wchar_t wfilepath[128];
	int iBufferSize;
	iBufferSize = MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));

	//テクスチャコンテナに同様のキーがあるか検索
	decltype(textures)::iterator it = textures.find(filename);
	
	//見つかったら
	if (it != textures.end()) {
		return &it->second;
	}
	else {
		//見つからなかったらテクスチャ生成
		DirectX::TexMetadata metadata{};
		DirectX::ScratchImage scratchImg{};
		DirectX::ScratchImage mipChain{};

		HRESULT result;
		result = LoadFromWICFile(
			wfilepath,
			WIC_FLAGS_NONE,
			&metadata, scratchImg);

		Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
		D3D12_RESOURCE_DESC textureResourceDesc{};

		//ファイルが見つかったら通常のファイル読み込み
		if (filename != "NULL") {
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
			D3D12_HEAP_PROPERTIES textureHeapProp{};
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
			result = dev->CreateCommittedResource(
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
		else {	//ファイルが見つからないorファイル名が読み込まれていないなら256*256のランダムの色のピクセルの画像データを入れる
			////横方向ピクセル数
			const size_t textureWidth = 256;
			//縦方向ピクセル数
			const size_t textureHeight = 256;
			//配列の要素数
			const size_t imageDataCount = textureWidth * textureHeight;
			//画像イメージデータ配列
			XMFLOAT4* imageData = new XMFLOAT4[imageDataCount]; //※必ず後で開放する

			//全ピクセルの色を初期化
			for (size_t i = 0; i < imageDataCount; i++) {
				imageData[i].x = Random(0.0f, 1.0f);	// R
				imageData[i].y = Random(0.0f, 1.0f); // G
				imageData[i].z = Random(0.0f, 1.0f); // B
				imageData[i].w = Random(0.0f, 1.0f); // A
			}

			//ヒープ設定
			D3D12_HEAP_PROPERTIES textureHeapProp{};
			textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
			textureHeapProp.CPUPageProperty =
				D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
			textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
			//リソース設定
			textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			textureResourceDesc.Width = textureWidth;	// 幅
			textureResourceDesc.Height = textureHeight;	// 高さ
			textureResourceDesc.DepthOrArraySize = 1;
			textureResourceDesc.MipLevels = 1;
			textureResourceDesc.SampleDesc.Count = 1;

			result = dev->CreateCommittedResource(
				&textureHeapProp,
				D3D12_HEAP_FLAG_NONE,
				&textureResourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&texBuff));

			//テクスチャバッファにデータ転送
			result = texBuff->WriteToSubresource(
				0,
				nullptr,				//全領域へコピー
				imageData,			//元データアドレス
				sizeof(XMFLOAT4) * textureWidth,	//1ラインサイズ
				sizeof(XMFLOAT4) * imageDataCount 	//全サイズ
			);
			delete[] imageData;
		}



		UINT incrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = descHeap->GetCPUDescriptorHandleForHeapStart();
		srvCpuHandle.ptr += incrementSize * textures.size();
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descHeap->GetGPUDescriptorHandleForHeapStart();
		srvGpuHandle.ptr += incrementSize * textures.size();

		//新しいテクスチャデータ作成
		TextureData newTexture{};
		newTexture.resource = texBuff;
		newTexture.cpuSRVHandle = srvCpuHandle;
		newTexture.gpuSRVHandle = srvGpuHandle;

		//SRV生成
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = textureResourceDesc.Format;//RGBA float
		srvDesc.Shader4ComponentMapping =
			D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
		srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

		//ハンドルの指す位置にシェーダーリソースビュー作成
		dev->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvCpuHandle);

		//コンテナに挿入
		textures.insert(std::make_pair(filename, newTexture));

		return &textures.find(filename)->second;

	}
}

void Texture::Initialize(ID3D12Device* device)
{
	HRESULT result;
	//デバイスのインスタンスを借りる
	dev = device;

	//デスクリプタヒープ生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = spriteSRVCount;
	result = dev->CreateDescriptorHeap(&descHeapDesc,
		IID_PPV_ARGS(&descHeap));
	assert(SUCCEEDED(result));

}


