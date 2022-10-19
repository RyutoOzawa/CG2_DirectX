#include "Sprite.h"
#include<d3dx12.h>
using namespace DirectX;

void Sprite::Create(ID3D12Device* device, int window_width, int window_height)
{
	HRESULT result = S_FALSE;

	//頂点データ
	VertexPosUv vertices[] = {
		//  x       y      z      u     v
		{{  0.0f, 100.0f, 0.0f},{0.0f,1.0f}},	//左下
		{{  0.0f,   0.0f, 0.0f},{0.0f,0.0f}},	//左上
		{{100.0f, 100.0f, 0.0f},{1.0f,1.0f}},	//右下
		{{100.0f,   0.0f, 0.0f},{1.0f,0.0f}},	//右上
	};


	//ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices));

	//頂点バッファ生成
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuff));

	//頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertexBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertexBuff->Unmap(0, nullptr);

	//頂点バッファビューの生成
	vbView.BufferLocation = vertexBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) * 0xff) & ~0xff);

	//定数バッファ
	result = device->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,
		IID_PPV_ARGS(&constBuff));

	//定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1);	//色指定(RGBA)
	//平行投影行列
	constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);
	constBuff->Unmap(0, nullptr);
}

void Sprite::BeginDraw(ID3D12GraphicsCommandList* cmdList, const PipelineSet& pipelineSet, ID3D12DescriptorHeap* descHeap)
{
	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelineSet.pipelineState.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(pipelineSet.rootSignature.Get());
	//プリミティブ形状の設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャ用デスクリプタヒープの設定
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void Sprite::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//頂点バッファセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff.Get()->GetGPUVirtualAddress());
	//描画
	cmdList->DrawInstanced(4, 1, 0, 0);
}
