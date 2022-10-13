#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include"Input.h"
#include<DirectXTex.h>
//#include"WindowsAPI.h"
//#include"DirectX.h"
#include"Object3d.h"
#include"Util.h"
#include"Texture.h"
#include"GpPipeline.h"
#include"Gridline.h"
#include<string>
using namespace Microsoft::WRL;


//パイプラインステートとルートシグネチャのセット
struct PipelineSet {
	//パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
};

PipelineSet CreatepipeLine3D(ID3D12Device* dev);



//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//windowsAPI初期化処理
	WindowsAPI* windowsAPI = new WindowsAPI();
	windowsAPI->Initialize();

	// DirectX初期化処理
	ReDirectX* directX = new ReDirectX();
	directX->Initialize(windowsAPI);

	HRESULT result{};

	//キーボード初期化処理
	Input* input = new Input();
	input->Initialize(windowsAPI);



	//ランダムな数値を取得
	float randValue = Random(-100, 100);

#pragma region 描画初期化処理

	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;//色（RGBA）
	};

	//ワールド変換行列
//	XMMATRIX matWorld0;
	//XMMATRIX matWorld1;

	const size_t kObjCount = 50;
	Object3d obj[kObjCount];

	Object3d object;

	XMMATRIX matProjection;
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, -100);	//視点座標
	XMFLOAT3 target(0, 0, 0);	//注視点座標
	XMFLOAT3 up(0, 1, 0);		//上方向ベクトル
	float angle = 0.0f;	//カメラの回転角
	float angle2 = 0.0f;	//カメラの回転角
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	XMFLOAT4 color_ = { 1.0f,1.0f,1.0f,0.0f };
	bool colorFlag = false;



	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;			//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	ComPtr<ID3D12Resource> constBufferDataMaterial;

	//配列内の全オブジェクトに対して
	for (int i = 0; i < _countof(obj); i++) {
		//初期化
		obj[i].Initialize(directX->GetDevice());

		obj[i].scale = { 1,1,1 };
		obj[i].rotation = { 0.0f,0.0f,0.0f };
		obj[i].position = { Random(-100, 100),Random(-100, 100),Random(-100, 100) };
	}

	object.Initialize(directX->GetDevice());

	//透視東映返還行列の計算
	//専用の行列を宣言
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),					//上下画角45度
		(float)WindowsAPI::winW / WindowsAPI::winH,	//アスペクト比（画面横幅/画面縦幅）
		0.1f, 1000.0f								//前橋、奥橋
	);


	//定数バッファ
	result = directX->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBufferDataMaterial));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBufferDataMaterial->Map(0, nullptr, (void**)&constMapMaterial);//マッピング
	assert(SUCCEEDED(result));

	//値を書き込むと自動的に転送される
	constMapMaterial->color = color_;		//RGBAで半透明の赤


	Texture texture1;
	texture1.LoadTexture(L"Resources/mario.jpg");
	texture1.Initialize(directX->GetDevice());

	Texture texture2;
	texture2.LoadTexture(L"Resources/reimu.png");
	texture2.Initialize(directX->GetDevice());

	Texture texture3;
	texture3.LoadTexture(L"Resources/orangeBlock.png");
	texture3.Initialize(directX->GetDevice());

	ComPtr<ID3D12Resource> texBuff;
	texBuff = texture1.texBuff;

	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定をもとにSRV用デスクリプタヒープを生成
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	result = directX->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
	//頂点データ構造体
	struct Vertex {
		XMFLOAT3 pos;//xyz座標
		XMFLOAT3 normal;//法線ベクトル
		XMFLOAT2 uv;//uv座標
	};

	// 頂点データ
	Vertex vertices[] = {
		//     x     y    z     法線  u    v

			//前
			{{-5.0f,-5.0f,-5.0f}, {},{0.0f,1.0f}}, // 左下
			{{-5.0f, 5.0f,-5.0f}, {},{0.0f,0.0f}}, // 左上
			{{ 5.0f,-5.0f,-5.0f}, {},{1.0f,1.0f}}, // 右下
			{{ 5.0f, 5.0f,-5.0f}, {},{1.0f,0.0f}}, // 右上

			//後
			{{-5.0f,-5.0f, 5.0f}, {},{0.0f,1.0f}}, // 左下
			{{-5.0f, 5.0f, 5.0f}, {},{0.0f,0.0f}}, // 左上
			{{ 5.0f,-5.0f, 5.0f}, {},{1.0f,1.0f}}, // 右下
			{{ 5.0f, 5.0f, 5.0f}, {},{1.0f,0.0f}}, // 右上

			//左
			{{-5.0f,-5.0f,-5.0f}, {} ,{0.0f,1.0f}}, // 左下
			{{-5.0f,-5.0f, 5.0f}, {} ,{0.0f,0.0f}}, // 左上
			{{-5.0f, 5.0f,-5.0f}, {} ,{1.0f,1.0f}}, // 右下
			{{-5.0f, 5.0f, 5.0f}, {} ,{1.0f,0.0f}}, // 右上

			//右
			{{ 5.0f,-5.0f,-5.0f}, {} ,{0.0f,1.0f}}, // 左下
			{{ 5.0f,-5.0f, 5.0f}, {} ,{0.0f,0.0f}}, // 左上
			{{ 5.0f, 5.0f,-5.0f}, {} ,{1.0f,1.0f}}, // 右下
			{{ 5.0f, 5.0f, 5.0f}, {} ,{1.0f,0.0f}}, // 右上

			//下
			{{-5.0f, 5.0f,-5.0f}, {},{0.0f,1.0f}}, // 左下
			{{ 5.0f, 5.0f,-5.0f}, {},{0.0f,0.0f}}, // 左上
			{{-5.0f, 5.0f, 5.0f}, {},{1.0f,1.0f}}, // 右下
			{{ 5.0f, 5.0f, 5.0f}, {},{1.0f,0.0f}}, // 右上

			//上
			{{-5.0f,-5.0f,-5.0f}, {},{0.0f,1.0f}}, // 左下
			{{ 5.0f,-5.0f,-5.0f}, {},{0.0f,0.0f}}, // 左上
			{{-5.0f,-5.0f, 5.0f}, {},{1.0f,1.0f}}, // 右下
			{{ 5.0f,-5.0f, 5.0f}, {},{1.0f,0.0f}}, // 右上
	};



	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

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
	ComPtr<ID3D12Resource> vertBuff;
	result = directX->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();


	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	srvDesc.Format = resDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	directX->GetDevice()->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);

	//GetDescriptorHandIncrementSizeの引数はD3D12_DESCRIPTOR_HEAP_TYPE
	//Heapの種類によってDescriptorのサイズは異なる(異なってもいいという仕様)
	UINT incrementSize = directX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	texture1.CreateSRV(directX->GetDevice(), srvHandle);
	//srvHandle.ptr += incrementSize;
	texture2.CreateSRV(directX->GetDevice(), srvHandle);
	//srvHandle.ptr += incrementSize;
	texture3.CreateSRV(directX->GetDevice(), srvHandle);

	//インデックスデータ
	unsigned short indices[] = {

		//前
		0,1,2,		//三角形1つ目
		2,1,3,		//三角形2つ目
		//後	
		5,4,7,		//三角形3つ目
		7,4,6,		//三角形4つ目
		//左
		8,9,10,		//三角形5つ目
		10,9,11,	//三角形6つ目
		//右
		13,12,15,	//三角形7つ目
		15,12,14,	//三角形8つ目
		//下
		17,16,19,	//三角形9つ目
		19,16,18,	//三角形10つ目
		//上
		20,21,22,	//三角形11つ目
		22,21,23,	//三角形12つ目
	};
	//法線の計算
	for (int i = 0; i < _countof(indices) / 3; i++) {
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices2].pos);
		//p0→p1ベクトル、p0→p2ベクトルを計算（ベクトルの減算）
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//正規化
		normal = XMVector3Normalize(normal);
		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[indices0].normal, normal);
		XMStoreFloat3(&vertices[indices1].normal, normal);
		XMStoreFloat3(&vertices[indices2].normal, normal);
	}

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;//インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの生成
	ComPtr<ID3D12Resource> indexBuff;
	result = directX->GetDevice()->CreateCommittedResource(
		&heapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];//インデックスをコピー
	}
	//マッピング解除
	indexBuff->Unmap(0, nullptr);

	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;



	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト


	//-----ココからスプライトと3Dオブジェクトでコピーを作る-----//

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl", // シェーダファイル名
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
		L"Resources/shaders/BasicPS.hlsl", // シェーダファイル名
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


	Gridline gridline{};
	gridline.start = { -100.0f,0.0f,-100.0f };
	gridline.end = { 100.0f,0.0f,100.0f };
	gridline.Initialize(directX->GetDevice(), 30, srvHandle);

	const int pipelineMax = 5;

	GpPipeline pipeline[pipelineMax];
	pipeline[BLEND_NOBLEND].SetPipeline(vsBlob, psBlob);
	pipeline[BLEND_ALPHA].SetPipeline(vsBlob, psBlob, BLEND_ALPHA);
	pipeline[BLEND_ADD].SetPipeline(vsBlob, psBlob, BLEND_ADD);
	pipeline[BLEND_SUB].SetPipeline(vsBlob, psBlob, BLEND_SUB);
	pipeline[BLEND_INV].SetPipeline(vsBlob, psBlob, BLEND_INV);

	//グリッド線描画用のパイプライン
	GpPipeline gridPipeline;
	gridPipeline.SetPipeline(vsBlob, psBlob, BLEND_NOBLEND, 1);
	//	gridPipeline.desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;


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
	result = directX->device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	for (int i = 0; i < pipelineMax; i++) {
		pipeline[i].desc.pRootSignature = rootSignature.Get();
	}
	gridPipeline.desc.pRootSignature = rootSignature.Get();


	// パイプランステートの生成
	ComPtr<ID3D12PipelineState> pipelineState;
	pipeline[0].SetPipelineState(directX->GetDevice(), pipelineState);
	//pipelineState = pipeline1.state;
	/*result = directX->device->CreateGraphicsPipelineState(&pipeline1.desc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));*/

	//-----ココまでスプライトと3Dオブジェクトでコピーを作る-----//

#pragma endregion



	// ゲームループ
	while (true) {

		//windowsのメッセージ処理
		if (windowsAPI->ProcessMessage()) {
			//ループを抜ける
			break;
		}

#pragma region DirectX毎フレーム処理
		// DirectX毎フレーム処理 ここから

		input->Update();

		//カメラ回転処理
		{
			if (input->IsPress(DIK_RIGHT) || input->IsPress(DIK_LEFT)) {
				if (input->IsPress(DIK_RIGHT))angle += XMConvertToRadians(1.0f);
				else if (input->IsPress(DIK_LEFT))angle -= XMConvertToRadians(1.0f);

				eye.x = -100 * sinf(angle);
				eye.z = -100 * cosf(angle);

			}

			eye.y = 25.0f;

			if (input->IsTrigger(DIK_SPACE)) {
				if (colorFlag)colorFlag = false;
				else colorFlag = true;


			}
		}

		if (!colorFlag) {
			color_.w = 0.5f;

			const float  colorSpd = 0.01f;


			//キー操作で色を変更
			if (color_.y < 0.5f) color_.x += colorSpd;
			else color_.x -= colorSpd;

			if (color_.z < 0.5f) color_.y += colorSpd;
			else color_.y -= colorSpd;

			if (color_.x < 0.5f) color_.z += colorSpd;
			else color_.z -= colorSpd;

			if (color_.x > 1.0f) {
				color_.x = 1.0f;
			}
			else if (color_.x < 0) {
				color_.x = 0;
			}

			if (color_.y > 1.0f) {
				color_.y = 1.0f;
			}
			else if (color_.y < 0) {
				color_.y = 0;
			}

			if (color_.z > 1.0f) {
				color_.z = 1.0f;
			}
			else if (color_.z < 0) {
				color_.z = 0;
			}



			color_.x = fmodf(color_.x, 1.0f);
			color_.y = fmodf(color_.y, 1.0f);
			color_.z = fmodf(color_.z, 1.0f);

		}
		else color_ = { 1.0f,1.0f,1.0f,0.5f };



		//値を書き込むと自動的に転送される
		constMapMaterial->color = color_;


		//オブジェクトの平行移動処理
		{
			//いずれかのキーを押していたら
			if (input->IsPress(DIK_W) || input->IsPress(DIK_S) || input->IsPress(DIK_A) || input->IsPress(DIK_D)) {
				//キーで移動
				if (input->IsPress(DIK_W))object.position.z += 1.0f;
				else if (input->IsPress(DIK_S))object.position.z -= 1.0f;
				if (input->IsPress(DIK_D))object.position.x += 1.0f;
				else if (input->IsPress(DIK_A))object.position.x -= 1.0f;
			}
		}

		//オブジェクトの回転処理
		{
			//いずれかのキーを押していたら
			if (input->IsPress(DIK_Q) || input->IsPress(DIK_E)) {
				//キーで回転
				if (input->IsPress(DIK_Q))object.rotation.y -= 0.05f;
				else if (input->IsPress(DIK_E))object.rotation.y += 0.05f;
			}
		}

		//ビュー変換行列の計算
		matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

		////定数バッファに転送
		//	constMapTransform1->mat = matWorld1 * matView * matProjection;

		for (int i = 0; i < _countof(obj); i++) {
			obj[i].Update(matView, matProjection);
		}

		object.Update(matView, matProjection);

		gridline.Update(matView, matProjection);

		//描画前処理
		directX->BeginDraw();

		pipeline[0].SetPipelineState(directX->GetDevice(), pipelineState);

		if (input->IsPress(DIK_1)) {
			pipeline[0].SetPipelineState(directX->GetDevice(), pipelineState);
		}
		else if (input->IsPress(DIK_2)) {
			pipeline[1].SetPipelineState(directX->GetDevice(), pipelineState);
		}
		else if (input->IsPress(DIK_3)) {
			pipeline[2].SetPipelineState(directX->GetDevice(), pipelineState);
		}
		else if (input->IsPress(DIK_4)) {
			pipeline[3].SetPipelineState(directX->GetDevice(), pipelineState);
		}
		else if (input->IsPress(DIK_5)) {
			pipeline[4].SetPipelineState(directX->GetDevice(), pipelineState);
		}


		// パイプラインステートとルートシグネチャの設定コマンド
		directX->GetCommandList()->SetPipelineState(pipelineState.Get());
		directX->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

		// プリミティブ形状の設定コマンド
		directX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// 頂点バッファビューの設定コマンド
		directX->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

		//インデックスバッファビューの設定コマンド
		directX->GetCommandList()->IASetIndexBuffer(&ibView);

		//定数バッファビュー(CBV)の設定コマンド
		directX->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBufferDataMaterial->GetGPUVirtualAddress());
		ID3D12DescriptorHeap* srvheaps = { srvHeap.Get() };
		//SRVヒープの設定コマンド
		directX->GetCommandList()->SetDescriptorHeaps(1, &srvheaps);

		////SRVヒープの先頭ハンドルを取得
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		//2枚目を指し示すようにしたSRVのハンドルをルートパラメータ1番に設定

		//0キーで画像を変える
		if (input->IsPress(DIK_0)) {
			//	srvGpuHandle.ptr += incrementSize;
		}

		//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
		directX->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		//	全オブジェクトについて処理
		for (int i = 0; i < _countof(obj); i++) {
			obj[i].Draw(directX->GetCommandList(), vbView, ibView, _countof(indices));
		}

		srvGpuHandle.ptr += incrementSize;

		//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
		directX->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		object.Draw(directX->GetCommandList(), vbView, ibView, _countof(indices));

		//パイプラインステートをグリッド線用に
		gridPipeline.SetPipelineState(directX->GetDevice(), pipelineState);
		directX->GetCommandList()->SetPipelineState(pipelineState.Get());
		directX->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
		gridline.Draw(directX->GetCommandList(), srvheaps);

#pragma endregion
		// ４．描画コマンドここまで
		directX->EndDraw();

		// DirectX毎フレーム処理 ここまで
#pragma endregion

	}

	//WindowsAPI終了処理
	windowsAPI->Finalize();

	delete windowsAPI;
	delete input;
	delete directX;

	return 0;
}

void MatrixUpdate()
{
}

PipelineSet CreatepipeLine3D(ID3D12Device* dev)
{

	HRESULT result;

	ComPtr<ID3DBlob> vsBlob;		// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;		// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob;	// エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl", // シェーダファイル名
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
		L"Resources/shaders/BasicPS.hlsl", // シェーダファイル名
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




	return PipelineSet();
}
