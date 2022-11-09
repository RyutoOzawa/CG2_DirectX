#include "Sprite.h"
using namespace DirectX;
#include<wrl.h>
using namespace Microsoft::WRL;
using namespace std;
#include"WindowsAPI.h"

void Sprite::Initialize(SpriteManager* spriteManager, const wchar_t filename[])
{
	this->spriteManager = spriteManager;
	HRESULT result;

	ComPtr<ID3D12Device> dev = spriteManager->directX->GetDevice();

	//頂点データ
	VertexPosUv vertices_[] = {
		//x    y     z   
		{{  0.0f,100.0f,0.0f},{0.0f,1.0f}},//左下
		{{  0.0f,  0.0f,0.0f},{0.0f,0.0f}},//左上
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}},//右下
		{{100.0f,  0.0f,0.0f},{1.0f,0.0f}},//右上
	};

	for (int i = 0; i < _countof(vertices_); i++) {
		vertices[i] = vertices_[i];
	}

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データ要素数
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosUv) * _countof(vertices_));

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファ生成
	result = dev.Get()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (int i = 0; i < _countof(vertices_); i++) {
		vertMap[i] = vertices_[i];
	}
	//つながりを解除
	vertBuff->Unmap(0, nullptr);


	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	//頂点１つ分のデータサイズ
	vbView.StrideInBytes = sizeof(VertexPosUv);

	//定数バッファの設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//定数バッファ生成
	result = dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

	//値を書き込むと自動的に転送される
	constMap->color = XMFLOAT4(1, 1, 1, 1);

	//行列にはとりあえず単位行列を代入
	constMap->mat = XMMatrixIdentity();

	constMap->mat.r[0].m128_f32[0] = 2.0f / WindowsAPI::winW;
	constMap->mat.r[1].m128_f32[1] = -2.0f / WindowsAPI::winH;
	constMap->mat.r[3].m128_f32[0] = -1.0;
	constMap->mat.r[3].m128_f32[1] = 1.0f;

	//画像ファイルの読み込み
	texture.LoadTexture(filename);

	//テクスチャの初期化
	texture.Initialize(dev.Get());

	//マネージャークラスのSRVヒープの先頭を取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = this->spriteManager->descHeap->GetCPUDescriptorHandleForHeapStart();

	//SRV作成
	texture.CreateSRV(dev.Get(), srvHandle);
}

void Sprite::Draw()
{
	//頂点バッファビューの設定
	spriteManager->directX->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	
	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { spriteManager->descHeap.Get()};
	spriteManager->directX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRVヒープの先頭ハンドル取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = spriteManager->descHeap->GetGPUDescriptorHandleForHeapStart();
	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	spriteManager->directX->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	//定数バッファビュー
	spriteManager->directX->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//描画コマンド
	spriteManager->directX->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
}

void Sprite::SetColor(DirectX::XMFLOAT4 color_)
{
	constMap->color = color_;
}

void Sprite::CreateConstBuff()
{
}
