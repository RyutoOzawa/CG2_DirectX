#include "Sprite.h"
using namespace DirectX;
#include<wrl.h>
using namespace Microsoft::WRL;
using namespace std;
#include"WindowsAPI.h"
#include<d3dx12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include"GpPipeline.h"
#include<d3d12.h>
#include<d3dx12.h>

//静的メンバ変数の実態
string Sprite::defaultTextureDirectoryPath = "Resources/";
ID3D12Device* Sprite::device = nullptr;
ID3D12GraphicsCommandList* Sprite::cmdList = nullptr;
ComPtr<ID3D12PipelineState> Sprite::pipelineState = nullptr;
ComPtr<ID3D12RootSignature> Sprite::rootSignature = nullptr;
Matrix4 Sprite::matProjection{};

void Sprite::StaticInitialize(ID3D12Device* device_, ID3D12GraphicsCommandList* cmdList_, int windowWidth, int windowHeight)
{
	//nullチェック
	assert(device_);
	assert(cmdList_);
	device = device_;
	cmdList = cmdList_;

	//パイプラインステート生成
	CreatePipeline2D();

	//射影行列作成
	matProjection = matProjection.CreateParallelProjection((float)windowWidth,(float)windowHeight);
	
}

void Sprite::BeginDraw()
{
	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelineState.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	//プリミティブ形状の設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

}

void Sprite::SetTextureCommand()
{
	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetDescHeap()};
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//SRVのハンドルをルートパラメータ1番に設定
	cmdList->SetGraphicsRootDescriptorTable(1, texData->gpuSRVHandle);
}

void Sprite::CreatePipeline2D()
{
	HRESULT result;
	ComPtr<ID3DBlob> vsBlob;		// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;		// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob;	// エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/SpriteVertexShader.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/SpritePixelShader.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//頂点レイアウトなど、グラフィックスパイプラインをまとめたやつの宣言、設定
	GpPipeline pipeline;
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

	inputLayout.push_back(
		{
		"POSITION",										//セマンティック名
		0,												//同じセマンティック名が複数ある時に使うインデックス（0でよい）
		DXGI_FORMAT_R32G32B32_FLOAT,					//要素数とビット数を表す（XYZの3つでfloat型なのでR32G32B32_FLOAT）
		0,												//入力スロットインデックス(0でよい)
		D3D12_APPEND_ALIGNED_ELEMENT,					//データのオフセット値(D3D12_APPEND_ALIGNED_ELEMENTだと自動設定)
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//入力データ種別(標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
		0												//一度に描画するインスタンス数(0でよい)
		});

	inputLayout.push_back({//uv座標
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		});

	pipeline.SetPipeline(vsBlob.Get(), psBlob.Get(), inputLayout);

	pipeline.SetBlendAlpha();

	//スプライトでは背面カリングしない
	pipeline.desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipeline.desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//深度ステンシルステートを設定しなおす
	pipeline.desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pipeline.desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;	//必ず上書き設定
	pipeline.desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pipeline.desc.DepthStencilState.DepthEnable = false;	//深度テストを行わない

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;		//一度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//すべてのシェーダから見える
	//定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams[2].Descriptor.ShaderRegister = 1;					//デスクリプタレンジ
	rootParams[2].Descriptor.RegisterSpace = 0;						//デスクリプタレンジ数
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見えるバッファE

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;				//横繰り返し（タイリング）
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;				//縦繰り返し（タイリング）
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;				//奥行繰り返し（タイリング）
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//すべてリニア補間
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダからのみ使用可能

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;	//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);		//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	//パイプラインにルートシグネチャをセット
	pipeline.desc.pRootSignature = rootSignature.Get();

	//パイプラインステートの生成
	pipeline.SetPipelineState(device, pipelineState);
}

Sprite::Sprite()
{
}

Sprite::Sprite(TextureData* texData, Vector2 pos, Vector2 size_, Vector4 color_, Vector2 anchorP, bool isFlipX_, bool isFlipY_):
texData(texData),position(pos),color(color_),anchorPoint(anchorP),size(size_),isFlipX(isFlipX_),isFlipY(isFlipY_)

{
}

void Sprite::Initialize(std::string filename)
{
	texData = Texture::LoadTexture(filename);

	Initialize(texData);
}

void Sprite::Initialize(TextureData* texData_)
{
	texData = texData_;

	HRESULT result{  };
	AdjustTextureSize();
	//テクスチャサイズをスプライトのサイズに適用
	size = textureSize;

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
	result = device->CreateCommittedResource(
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
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];
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
	result = device->CreateCommittedResource(
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
	constMap->color = Vector4(1, 1, 1, 1);

	//行列にはとりあえず単位行列を代入
	matWorld.identity();
	constMap->mat = matWorld;
}

void Sprite::Draw()
{
	//非表示なら処理終了
	if (isInvisible) {
		return;
	}

	//テクスチャの設定コマンド
	SetTextureCommand();

	Update();

	//頂点バッファビューの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファビュー
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//描画コマンド
	cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
}

void Sprite::Update()
{
	HRESULT result;

	//頂点データをメンバ変数で計算
	float left = (0.0f - anchorPoint.x) * size.x;
	float right = (1.0f - anchorPoint.x) * size.x;
	float top = (0.0f - anchorPoint.y) * size.y;
	float bottom = (1.0f - anchorPoint.y) * size.y;

	//左右反転
	if (isFlipX) {
		left = -left;
		right = -right;
	}

	//上下反転
	if (isFlipY) {
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,bottom,0.0f };
	vertices[LT].pos = { left,top,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,top,0.0f };

	//テクスチャバッファ取得
	ID3D12Resource* textureBuffer = texData->resource.Get();
	//指定番号のテクスチャが読み込み済みなら
	if (textureBuffer) {
		//テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

		float tex_left = textureLeftTop.x / resDesc.Width;
		float tex_right = (textureLeftTop.x + textureSize.x) / resDesc.Width;
		float tex_top = textureLeftTop.y / resDesc.Height;
		float tex_bottom = (textureLeftTop.y + textureSize.y) / resDesc.Height;
		//頂点のUVに反映
		vertices[LB].uv = { tex_left,tex_bottom };
		vertices[LT].uv = { tex_left,tex_top };
		vertices[RB].uv = { tex_right,tex_bottom };
		vertices[RT].uv = { tex_right,tex_top };

	}

	//頂点バッファにデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];
	}
	//つながりを解除
	vertBuff->Unmap(0, nullptr);

	matWorld.identity();
	Matrix4 matRot,matTrans;
	matWorld *= matRot.rotateZ(rotation);
	matWorld *= matTrans.translate({ position.x,position.y ,0.0f});

	matWorld *= matProjection;

	constMap->mat = matWorld;

	//色を転送
	constMap->color = color;
}

void Sprite::AdjustTextureSize() {
	ID3D12Resource* textureBuffer = texData->resource.Get();
	assert(textureBuffer);

	//テクスチャ情報取得
	D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

	textureSize.x = static_cast<float>(resDesc.Width);
	textureSize.y = static_cast<float>(resDesc.Height);
}



