﻿#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include"Input.h"
#include<DirectXTex.h>
//#include"WindowsAPI.h"
//#include"DirectX.h"

WindowsAPI windowsAPI;


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//windowsAPI初期化処理
	windowsAPI.Initialize();

// DirectX初期化処理
	DirectXInit directX;
	directX.Initialize(windowsAPI);
	Input input;
	HRESULT result{};

	//キーボード初期化処理
	input.Initialize(result, windowsAPI);

#pragma region 描画初期化処理

	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;//色（RGBA）
	};

	//定数バッファ用データ構造体(3D変換行列)
	struct ConstBufferDataTransform {
		XMMATRIX mat;	//3D変換行列
	};


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

	ID3D12Resource* constBufferDataMaterial = nullptr;

	ID3D12Resource* constBuffTransform = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;
	{
		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{  };
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用
		//リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	//256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//定数バッファの生成
		result = directX.device->CreateCommittedResource(
			&cbHeapProp,//ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,//リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));
		assert(SUCCEEDED(result));
		//定数バッファのマッピング
		result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);//マッピング
		assert(SUCCEEDED(result));
		//単位行列を代入
		constMapTransform->mat = XMMatrixIdentity();
		constMapTransform->mat.r[0].m128_f32[0] = 2.0f / windowsAPI.winW;
		constMapTransform->mat.r[1].m128_f32[1] = -2.0f / windowsAPI.winH;
		constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
		constMapTransform->mat.r[3].m128_f32[1] = 1.0f;
	}


	//定数バッファ
	result = directX.device->CreateCommittedResource(
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
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1.0f);		//RGBAで半透明の赤

	////横方向ピクセル数
	//const size_t textureWidth = 256;
	////縦方向ピクセル数
	//const size_t textureHeight = 256;
	////配列の要素数
	//const size_t imageDataCount = textureWidth * textureHeight;
	////画像イメージデータ配列
	//XMFLOAT4* imageData = new XMFLOAT4[imageDataCount]; //※必ず後で開放する

	////全ピクセルの色を初期化
	//for (size_t i = 0; i < imageDataCount; i++) {
	//	imageData[i].x = 1.0f;	// R
	//	imageData[i].y = 0.0f;	// G
	//	imageData[i].z = 0.0f;	// B
	//	imageData[i].w = 1.0f;	// A
	//}

	TexMetadata metadata{  };
	ScratchImage scratchImg{};
	//WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resources/mario.jpg",	//「Resourcesq」フォルダの「mario.jpg」
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	ScratchImage mipChain{};
	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);


	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
	D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;	// 幅
	textureResourceDesc.Height = (UINT)metadata.height;	// 高さ
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	ID3D12Resource* texBuff = nullptr;
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

	//元データ開放
	//delete[] imageData;

	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定をもとにSRV用デスクリプタヒープを生成
	ID3D12DescriptorHeap* srvHeap = nullptr;
	result = directX.device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//頂点データ構造体
	struct Vertex {
		XMFLOAT3 pos;//xyz座標
		XMFLOAT2 uv;//uv座標
	};

	// 頂点データ
	Vertex vertices[] = {
		//     x     y    z       u    v
			{{  0.0f,100.0f,0.0f} ,{0.0f,1.0f}}, // 左下
			{{  0.0f,  0.0f,0.0f} ,{0.0f,0.0f}}, // 左上
			{{100.0f,100.0f,0.0f} ,{1.0f,1.0f}}, // 右下
			{{100.0f,  0.0f,0.0f} ,{1.0f,0.0f}}, // 右上

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
	ID3D12Resource* vertBuff = nullptr;
	result = directX.device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	srvDesc.Format = resDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	directX.device->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);


	//インデックスデータ
	unsigned short indices[] = {
		0,1,2,//三角形1つ目
		1,2,3,//三角形2つ目
	};

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
	ID3D12Resource* indexBuff = nullptr;
	result = directX.device->CreateCommittedResource(
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
	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicVS.hlsl", // シェーダファイル名
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
		L"BasicPS.hlsl", // シェーダファイル名
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

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{//xyz座標
	"POSITION",										//セマンティック名
	0,												//同じセマンティック名が複数ある時に使うインデックス（0でよい）
	DXGI_FORMAT_R32G32B32_FLOAT,					//要素数とビット数を表す（XYZの3つでfloat型なのでR32G32B32_FLOAT）
	0,												//入力スロットインデックス(0でよい)
	D3D12_APPEND_ALIGNED_ELEMENT,					//データのオフセット値(D3D12_APPEND_ALIGNED_ELEMENTだと自動設定)
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//入力データ種別(標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
	0												//一度に描画するインスタンス数(0でよい)
	},
	{//uv座標
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
		//座標以外に、色、テクスチャUVなどを渡す場合はさらに続ける
	};

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA全てのチャンネルを描画

	//共通設定（アルファ値）
	blenddesc.BlendEnable = true;					//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う

	////加算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;	//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;	//デストの値を100%使う

	////減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;				//デストの値を100%使う

	////色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	//1.0f-デストカラーの色
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;				//使わない

	//半透明合成（アルファブレンド）
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-ソースのアルファ値

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

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
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU  = D3D12_TEXTURE_ADDRESS_MODE_WRAP;				//横繰り返し（タイリング）
	samplerDesc.AddressV  = D3D12_TEXTURE_ADDRESS_MODE_WRAP;				//縦繰り返し（タイリング）
	samplerDesc.AddressW  = D3D12_TEXTURE_ADDRESS_MODE_WRAP;				//奥行繰り返し（タイリング）
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//すべてリニア補間
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;				
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダからのみ使用可能
	
	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;
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
	result = directX.device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;

	// パイプランステートの生成
	ID3D12PipelineState* pipelineState = nullptr;
	result = directX.device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
#pragma endregion

	// ゲームループ
	while (true) {

#pragma region ウィンドウメッセージ処理
		// メッセージがある?
		if (PeekMessage(&windowsAPI.msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&windowsAPI.msg); // キー入力メッセージの処理
			DispatchMessage(&windowsAPI.msg); // プロシージャにメッセージを送る
		}
		// ✖ボタンで終了メッセージが来たらゲームループを抜ける
		if (windowsAPI.msg.message == WM_QUIT) {
			break;
		}
#pragma endregion

#pragma region DirectX毎フレーム処理
		// DirectX毎フレーム処理 ここから

		input.Update();

	

		// バックバッファの番号を取得（2つなので0番か1番）
		UINT bbIndex = directX.swapChain->GetCurrentBackBufferIndex();

		// １．リソースバリアで書き込み可能に変更
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = directX.backBuffers[bbIndex]; // バックバッファを指定
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;      // 表示状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
		directX.commandList->ResourceBarrier(1, &barrierDesc);
		// ２．描画先の変更
		// レンダーターゲットビューのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = directX.rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * directX.device->GetDescriptorHandleIncrementSize(directX.rtvHeapDesc.Type);
		directX.commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
		// ３．画面クリア           R     G     B    A
		FLOAT clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色
		directX.commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		if (input.IsPress(DIK_SPACE)) {//スペースキーが押されていたら
			FLOAT	clearColor[] = { 1.0f,0.0f,1.0f,0.0f };
			directX.commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		}
		// ４．描画コマンドここから
#pragma region グラフィックスコマンド
// ビューポート設定コマンド
		D3D12_VIEWPORT viewport{};
		viewport.Width = windowsAPI.winW;
		viewport.Height = windowsAPI.winH;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		// ビューポート設定コマンドを、コマンドリストに積む
		directX.commandList->RSSetViewports(1, &viewport);

		// シザー矩形
		D3D12_RECT scissorRect{};
		scissorRect.left = 0; // 切り抜き座標左
		scissorRect.right = scissorRect.left + windowsAPI.winW; // 切り抜き座標右
		scissorRect.top = 0; // 切り抜き座標上
		scissorRect.bottom = scissorRect.top + windowsAPI.winH; // 切り抜き座標下
		// シザー矩形設定コマンドを、コマンドリストに積む
		directX.commandList->RSSetScissorRects(1, &scissorRect);

		// パイプラインステートとルートシグネチャの設定コマンド
		directX.commandList->SetPipelineState(pipelineState);
		directX.commandList->SetGraphicsRootSignature(rootSignature);

		// プリミティブ形状の設定コマンド
		directX.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// 頂点バッファビューの設定コマンド
		directX.commandList->IASetVertexBuffers(0, 1, &vbView);

		//インデックスバッファビューの設定コマンド
		directX.commandList->IASetIndexBuffer(&ibView);

		//定数バッファビュー(CBV)の設定コマンド
		directX.commandList->SetGraphicsRootConstantBufferView(0, constBufferDataMaterial->GetGPUVirtualAddress());
		//SRVヒープの設定コマンド
		directX.commandList->SetDescriptorHeaps(1, &srvHeap);
		//SRVヒープの先頭ハンドルを取得
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
		directX.commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
		//定数バッファビュー(CBV)の設定コマンド
		directX.commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

		// 描画コマンド
		directX.commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
#pragma endregion
		// ４．描画コマンドここまで
		// ５．リソースバリアを戻す
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;        // 表示状態へ
		directX.commandList->ResourceBarrier(1, &barrierDesc);

		// 命令のクローズ
		result = directX.commandList->Close();
		assert(SUCCEEDED(result));
		// コマンドリストの実行
		ID3D12CommandList* commandLists[] = { directX.commandList };
		directX.commandQueue->ExecuteCommandLists(1, commandLists);

		// 画面に表示するバッファをフリップ（裏表の入替え）
		result = directX.swapChain->Present(1, 0);
		assert(SUCCEEDED(result));

		// コマンドの実行完了を待つ
		directX.commandQueue->Signal(directX.fence, ++directX.fenceVal);
		if (directX.fence->GetCompletedValue() != directX.fenceVal) {
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			directX.fence->SetEventOnCompletion(directX.fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}

		// キューをクリア
		result = directX.commandAllocator->Reset();
		assert(SUCCEEDED(result));
		// 再びコマンドリストを貯める準備
		result = directX.commandList->Reset(directX.commandAllocator, nullptr);
		assert(SUCCEEDED(result));
		// DirectX毎フレーム処理 ここまで
#pragma endregion



	}
	// ウィンドウクラスを登録解除
	UnregisterClass(windowsAPI.w.lpszClassName, windowsAPI.w.hInstance);

	//コンソールへの文字入力
	//OutputDebugStringA("Hello,DirectX!!");

	return 0;
}