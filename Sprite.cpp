#include "Sprite.h"
using namespace DirectX;
#include<wrl.h>
using namespace Microsoft::WRL;

void Sprite::Initialize(SpriteManager* spriteManager)
{
	this->spriteManager = spriteManager;
	HRESULT result;

	ComPtr<ID3D12Device> dev = spriteManager->directX->GetDevice();

	//頂点データ
	XMFLOAT3 vertices_[] = {
		{-0.5f,-0.5f,0.0f},//左下
		{-0.5f,+0.5f,0.0f},//左上si
		{+0.5f,-0.5f,0.0f},//右下
		{+0.5f,+0.5f,0.0f},//右上
	};

	for (int i = 0; i < _countof(vertices_); i++) {
		vertices[i] = vertices_[i];
	}

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データ要素数
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices_));

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
	XMFLOAT3* vertMap = nullptr;
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
	vbView.StrideInBytes = sizeof(XMFLOAT3);


}

void Sprite::Draw()
{
	//頂点バッファビューの設定
	spriteManager->directX->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//描画コマンド
	spriteManager->directX->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
}
