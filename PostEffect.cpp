#include "PostEffect.h"
#include"WindowsAPI.h"
#include<d3dx12.h>

PostEffect::PostEffect() :Sprite(
	100,			//テクスチャ番号
	{0,0},			//座標
	{500.0f,500.0f},//サイズ
	{1,1,1,1},		//色
	{0.0f,0.0f},	//アンカーポイント
	false,			//左右反転フラグ
	false			//上下反転フラグ
)
{

}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//非表示なら処理終了
	if (isInvisible) {
		return;
	}

	size = { 500,500 };

	Update();

	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelineState.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャの設定コマンド
//	SetTextureCommand(textureIndex);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//頂点バッファビューの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファビュー
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//描画コマンド
	cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
}

void PostEffect::Initialize(uint32_t textureHandle)
{
	HRESULT result;

	//基底クラスとしての初期化
	Sprite::Initialize(textureHandle);

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WindowsAPI::winW,
		(UINT)WindowsAPI::winH,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//テクスチャバッファの生成
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff));
	assert(SUCCEEDED(result));

	//テクスチャを赤クリア
	//画素数(1280 * 720 = 921600ピクセル)
	const UINT pixelCount = WindowsAPI::winW * WindowsAPI::winH;
	//画像1行分のデータサイズ
	const UINT rowPitch = sizeof(UINT) * WindowsAPI::winW;
	//画像全体のデータサイズ
	const UINT depthPitch = rowPitch * WindowsAPI::winH;
	//画像イメージ
	UINT* img = new UINT[pixelCount];
	for (int i = 0; i < pixelCount; i++) { img[i] = 0xff0000ff; }

	//テクスチャバッファにデータ転送
	result = texBuff->WriteToSubresource(0, nullptr,
		img, rowPitch, depthPitch);
	assert(SUCCEEDED(result));
	delete[] img;

	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRV生成
	device->CreateShaderResourceView(texBuff.Get(),
		&srvDesc, descHeapSRV->GetCPUDescriptorHandleForHeapStart());


}
