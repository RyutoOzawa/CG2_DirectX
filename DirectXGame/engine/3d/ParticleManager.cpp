#include "ParticleManager.h"
using namespace Microsoft::WRL;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include"GpPipeline.h"
#include"Util.h"
#include"Texture.h"

ComPtr<ID3D12PipelineState> ParticleManager::pipelineState;
ComPtr<ID3D12RootSignature> ParticleManager::rootSignature;
ReDirectX* ParticleManager::directX = nullptr;
Camera* ParticleManager::camera = nullptr;

void ParticleManager::StaticInitialize(ReDirectX* directX_)
{
	assert(directX_);
	directX = directX_;

	//パイプライン設定
	CreatePipeline3D();
}

void ParticleManager::BeginDraw(Camera* camera_)
{
	camera = camera_;

	//パイプラインステートの設定
	directX->GetCommandList()->SetPipelineState(pipelineState.Get());
	//ルートシグネチャの設定
	directX->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	//プリミティブ形状の設定
	directX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	//2番定数バッファビューにカメラの定数バッファを設定
	directX->GetCommandList()->SetGraphicsRootConstantBufferView(2, camera->constBuff->GetGPUVirtualAddress());

}

void ParticleManager::Initialize(TextureData* texData)
{
	//テクスチャセット
	SetTexture(texData);

	ID3D12Device* device = directX->GetDevice();

	HRESULT result;
	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{  };
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//定数バッファの生成
	result = device->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0));
	assert(SUCCEEDED(result));
	//定数バッファのマッピング
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);//マッピング
	assert(SUCCEEDED(result));

	Matrix4 matrix = matrix.identity();
	constMap->matBillboard = matrix;

	

	//頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosScale) * vertexCount);

	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// 頂点バッファの生成
	result = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexPosScale* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	//for (int i = 0; i < vertices.size(); i++) {
	//	vertMap[i] = vertices[i]; // 座標をコピー
	//}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(VertexPosScale);
}

void ParticleManager::Update()
{
	//寿命がつきたパーティクルを全削除
	particles.remove_if([](Particle& x) {return x.frame > x.num_frame; });

	//全パーティクル更新
	for (std::forward_list<Particle>::iterator it = particles.begin(); it != particles.end(); it++) {
		//経過フレーム数をカウント
		it->frame++;
		//速度に加速度を加算
		it->velocity += it->accel;
		//速度による移動
		it->position += it->velocity;
		//パーティクルの現在時間を計算
		float f = (float)it->frame / it->num_frame;
		//スケールの線形補間
		it->scale = (it->scaleEnd - it->scaleStart) * f;
		it->scale += it->scaleStart;

	}
	//頂点バッファへデータ転送
	VertexPosScale* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		//パーティクルの情報を1つずつ反映
		for (std::forward_list<Particle>::iterator it = particles.begin(); it != particles.end(); it++) {
			//座標
			vertMap->pos = it->position;
			//スケール
			vertMap->scale = it->scale;
			//次の順番へ
			vertMap++;
		}
		vertBuff->Unmap(0, nullptr);
	}
}

void ParticleManager::Draw()
{
	matBillboard.identity();

	//カメラの各ベクトルを設定
	Vector3 eye, target, up;
	eye = camera->eye;
	target = camera->target;
	up = camera->up;
	Vector3 cameraAxisZ, cameraAxisX, cameraAxisY;
	cameraAxisZ = target - eye;
	cameraAxisZ.normalize();
	cameraAxisX = up.cross(cameraAxisZ);
	cameraAxisX.normalize();
	cameraAxisY = cameraAxisZ.cross(cameraAxisX);
	cameraAxisY.normalize();

	Matrix4 matCameraRot = {
		cameraAxisX.x,cameraAxisX.y,cameraAxisX.z,0,
		cameraAxisY.x,cameraAxisY.y,cameraAxisY.z,0,
		cameraAxisZ.x,cameraAxisZ.y,cameraAxisZ.z,0,
					0,            0,            0,1,
	};

	//カメラの行列をビルボード行列に
	matBillboard = matCameraRot;

	constMap->matBillboard = matBillboard;


	ID3D12GraphicsCommandList* commandList = directX->GetCommandList();
	//h定数バッファビュー(CBV)の設定コマンド
	commandList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());

	//モデルデータの描画用コマンドのまとまり
	//model->Draw(commandList);

	//頂点バッファビューの設定
	commandList->IASetVertexBuffers(0, 1, &vbView);

	//デスクリプタヒープの配列セット
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetDescHeap() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//ハンドルをセット
	commandList->SetGraphicsRootDescriptorTable(1, texData->gpuSRVHandle);
	//描画コマンド
	commandList->DrawInstanced((UINT)std::distance(particles.begin(),particles.end()), 1, 0, 0);
 }

void ParticleManager::Add(int life, const Vector3& position, const Vector3& velocity, const Vector3& accel, float scaleStart, float scaleEnd) {
	//パーティクルの数が最大なら追加しない
	if (std::distance(particles.begin(), particles.end()) >= vertexCount) {
		return;
	}
	
	//リストに要素を追加
	particles.emplace_front();
	//追加した要素の参照
	Particle& p = particles.front();
	//値のセット
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	p.scale = scaleStart;
	p.scaleStart = scaleStart;
	p.scaleEnd = scaleEnd;
}

void ParticleManager::AddLerp(float t, const Vector3& start, const Vector3& end, float scaleStart, float scaleEnd)
{//パーティクルの数が最大なら追加しない
	if (std::distance(particles.begin(), particles.end()) >= vertexCount) {
		return;
	}

	//リストに要素を追加
	particles.emplace_front();
	//追加した要素の参照
	Particle& p = particles.front();
	p.start = start;
	p.end = end;
	p.scaleStart = scaleStart;
	p.scaleEnd = scaleEnd;
}

void ParticleManager::CreatePipeline3D()
{
	HRESULT result;

	ID3D12Device* dev = directX->GetDevice();

	ComPtr<ID3DBlob> vsBlob;		// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob;		// ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;		// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob;	// エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/ParticleVS.hlsl", // シェーダファイル名
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

	// ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/ParticleGS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
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
		L"Resources/shaders/ParticlePS.hlsl", // シェーダファイル名
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

	//パイプライン設定
	GpPipeline pipeline3D;
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

	inputLayout.push_back(
		{
			"TEXCOORD",0,DXGI_FORMAT_R32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		});

	pipeline3D.SetPipeline(vsBlob.Get(), psBlob.Get(), inputLayout);
	//ジオメトリシェーダーの設定を追加
	pipeline3D.desc.GS.BytecodeLength = gsBlob->GetBufferSize();
	pipeline3D.desc.GS.pShaderBytecode = gsBlob->GetBufferPointer();

	pipeline3D.desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	//デプスの書き込み禁止
	pipeline3D.desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	pipeline3D.SetBlendAdd();
	//pipeline3D.SetBlendSub();

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;		//一度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//定数バッファ0番(ビルボード行列の定数)
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//すべてのシェーダから見える
	//定数バッファ1番(カメラ行列)
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams[2].Descriptor.ShaderRegister = 1;					//デスクリプタレンジ
	rootParams[2].Descriptor.RegisterSpace = 0;						//デスクリプタレンジ数
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見えるバッファ

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
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	//パイプラインにルートシグネチャをセット
	pipeline3D.desc.pRootSignature = rootSignature.Get();

	//パイプラインステートの生成
	pipeline3D.SetPipelineState(dev, pipelineState);
}

