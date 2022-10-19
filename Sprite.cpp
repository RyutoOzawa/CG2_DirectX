#include "Sprite.h"
#include<d3dx12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include"GpPipeline.h"
using namespace DirectX;
using namespace Microsoft::WRL;

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

	//デスクリプタヒープ生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = spriteSRVCount;
	result = device->CreateDescriptorHeap(&descHeapDesc,
		IID_PPV_ARGS(&descHeap));

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

	//射影行列をメンバ変数にコピー
	matProjection = constMap->mat;
}

void Sprite::BeginDraw(ID3D12GraphicsCommandList* cmdList, ID3D12Device* dev)
{
	//パイプライン生成
	pipelineSet = CreatePipeline2D(dev);

	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelineSet.pipelineState.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(pipelineSet.rootSignature.Get());
	//プリミティブ形状の設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャ用デスクリプタヒープの設定
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get()};
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void Sprite::Draw(ID3D12GraphicsCommandList* cmdList,ID3D12Device* dev)
{
	//ワールド行列更新
	matworld = XMMatrixIdentity();
	//Z軸回転
	matworld *= XMMatrixRotationZ(XMConvertToRadians(rotatiton));
	//平行移動
	matworld *= XMMatrixTranslation(position.x, position.y, position.z);
	
	//定数バッファの転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matworld * matProjection;
	constMap->color = color;
	constBuff->Unmap(0, nullptr);

	//頂点バッファセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff.Get()->GetGPUVirtualAddress());
	//描画
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::LoadTexture(UINT texnumber, const wchar_t* filename, ID3D12Device* dev)
{
	//異常な番号の指定を検出
	assert(texnumber <= spriteSRVCount - 1);

	HRESULT result;

	CD3DX12_CPU_DESCRIPTOR_HANDLE srvhandle(descHeap->GetCPUDescriptorHandleForHeapStart(),texnumber,
		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);

	Texture texture;
	texture.LoadTexture(filename);
	texture.Initialize(dev);
	//テクスチャバッファをtexnumber番目のテクスチャバッファでコピー
	texture.texBuff = texbuff[texnumber];
	//texnumber番目にSRV作成
	texture.CreateSRV(dev,srvhandle);

}

PipelineSet Sprite::CreatePipeline2D(ID3D12Device* dev)
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

	//スプライトでは背面カリングしない
	pipeline.desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//深度ステンシルステートを設定しなおす
	pipeline.desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;	//必ず上書き設定
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

	// ルートシグネチャ
		//パイプラインとルートシグネチャのセット
	PipelineSet pipelineSet;

	ComPtr<ID3D12RootSignature> rootSignature;
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;	//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);		//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootSignature));
	assert(SUCCEEDED(result));

	//パイプラインにルートシグネチャをセット
	pipeline.desc.pRootSignature = pipelineSet.rootSignature.Get();

	//パイプラインステートの生成
	pipeline.SetPipelineState(dev, pipelineSet.pipelineState);

	return pipelineSet;
}
