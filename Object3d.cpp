#include "Object3d.h"

#include"WindowsAPI.h"
#include<cassert>
using namespace DirectX;
using namespace Microsoft::WRL;
#include<d3dx12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include"GpPipeline.h"
#include"Texture.h"
#include<fstream>
#include<sstream>

#include<vector>
using namespace std;

//静的メンバ変数
ComPtr<ID3D12PipelineState> Object3d::pipelineState;
ComPtr<ID3D12RootSignature> Object3d::rootSignature;
ReDirectX* Object3d::directX = nullptr;
Object3d::Material Object3d::material;


Object3d::Object3d()
{
}

Object3d::~Object3d()
{
}

void Object3d::StaticInitialize(ReDirectX* directX_)
{
	//nullチェック
	assert(directX_);
	//メンバに渡す
	directX = directX_;
	//3D用パイプラインステート生成
	CreatePipeline3D();
}

void Object3d::BeginDraw()
{
	//パイプラインステートの設定
	directX->GetCommandList()->SetPipelineState(pipelineState.Get());
	//ルートシグネチャの設定
	directX->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	//プリミティブ形状の設定
	directX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
}

void Object3d::CreateModel(const std::string& modelname)
{
	HRESULT result;

	if (modelname != "NULL") {
		HRESULT result = S_FALSE;

		//ファイルストリーム
		ifstream file;
		//.objファイルを開く
		//file.open("Resources/triangle/triangle.obj");
	//	const string modelname = "triangle_mat";
		const string filename = modelname + ".obj";
		const string directoryPath = "Resources/" + modelname + "/";
		file.open(directoryPath + filename);

		//ファイルオープン失敗をチェック
		if (file.fail()) {
			assert(0);
		}

		vector<XMFLOAT3> positions;	//頂点座標
		vector<XMFLOAT3> normals;	//法線ベクトル
		vector<XMFLOAT2> texcords;	//テクスチャUV
		//1行ずつ読み込む
		string line;
		while (getline(file, line)) {

			//1行分の文字列をストリームに変換して解析しやすくする
			istringstream line_stream(line);

			//半角スペース区切りで行の先頭文字列を取得
			string key;
			getline(line_stream, key, ' ');

			//先頭文字列がmtllibならマテリアル
			if (key == "mtllib") {
				//マテリアルのファイル名読み込み
				string filename;
				line_stream >> filename;
				//マテリアル読み込み
				LoadMaterial(directoryPath,filename);
			}
			//先頭文字列がvなら頂点座標
			if (key == "v") {
				//X,Y,Z座標読み込み
				XMFLOAT3 position{};
				line_stream >> position.x;
				line_stream >> position.y;
				line_stream >> position.z;
				//座標データに追加
				positions.emplace_back(position);
				//頂点データに追加
			/*	Vertex vertex{};
				vertex.pos = position;
				vertices.emplace_back(vertex);*/
			}
			//先頭文字列がvtならテクスチャ
			if (key == "vt") {
				//U,V成分読み込み
				XMFLOAT2 texcoord{};
				line_stream >> texcoord.x;
				line_stream >> texcoord.y;
				//V方向反転
				texcoord.y = 1.0f - texcoord.y;
				//テクスチャ座標データに追加
				texcords.emplace_back(texcoord);
			}
			//先頭文字列がvnなら法線ベクトル
			if (key == "vn") {
				//X,Y,Z成分読み込み
				XMFLOAT3 normal{};
				line_stream >> normal.x;
				line_stream >> normal.y;
				line_stream >> normal.z;
				//法線ベクトルデータに追加
				normals.emplace_back(normal);
			}
			//先頭文字列がfならポリゴン(三角形)
			if (key == "f") {
				//半角スペース区切りで行の続きを読み込む
				string index_string;
				while (getline(line_stream, index_string, ' ')) {
					//頂点インデックス1個分の文字列をストリームに変換して解析しやすくなる
					istringstream index_stream(index_string);
					unsigned short indexPosition,indexNormal,indexTexcoord;
					index_stream >> indexPosition;
					index_stream.seekg(1, ios_base::cur);//スラッシュを飛ばす
					index_stream >> indexTexcoord;
					index_stream.seekg(1, ios_base::cur);//スラッシュを飛ばす
					index_stream >> indexNormal;
					//頂点データの追加
					Vertex vertex{};
					vertex.pos = positions[indexPosition - 1];
					vertex.normal = normals[indexNormal - 1];
					vertex.uv = texcords[indexTexcoord - 1];
					vertices.emplace_back(vertex);
					//頂点インデックスに追加
					indices.emplace_back((unsigned short)indices.size());
				}

			}
	
		}
		//ファイルを閉じる
		file.close();



	}
	else {
		//頂点データと頂点バッファビューの生成
		// 頂点データ
		Vertex vertices_[] = {
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
		for (int i = 0; i < _countof(vertices_); i++) {
			vertices.push_back(vertices_[i]);
		}

		//インデックスデータ
		unsigned short indices_[] = {

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

		for (int i = 0; i < _countof(indices_); i++) {
			indices.push_back(indices_[i]);
		}

		//法線の計算
		for (int i = 0; i < indices.size() / 3; i++) {
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

	}



	//頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

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
	result = directX->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	





	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(indices[0]) * indices.size());

	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;//インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの生成
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
	for (int i = 0; i < indices.size(); i++) {
		indexMap[i] = indices[i];//インデックスをコピー
	}
	//マッピング解除
	indexBuff->Unmap(0, nullptr);

	//インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;


}

void Object3d::Initialize(const std::string& filename)
{
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

	cbResourceDesc.Width = (sizeof(ConstBufferDatab1) + 0xff) & ~0xff;	//256バイトアラインメント
	//定数バッファの生成
	result = device->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1));
	assert(SUCCEEDED(result));
	//定数バッファへデータ転送
	
	result = constBuffB1->Map(0, nullptr, (void**)&constMapB1);
	constMapB1->ambient = material.ambient;
	constMapB1->diffuse = material.diffuse;
	constMapB1->specular = material.specular;
	constMapB1->alpha = material.alpha;
	constBuffB1->Unmap(0, nullptr);

	//モデルの生成
	CreateModel(filename);
	XMMATRIX matrix = XMMatrixIdentity();
	constMap->mat = matrix;


}

void Object3d::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	XMMATRIX matScale, matRot, matTrans;

	//各行列計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);	//Z軸周りに回転
	matRot *= XMMatrixRotationX(rotation.x);	//X軸周りに回転
	matRot *= XMMatrixRotationY(rotation.y);	//Y軸周りに回転
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();
	matWorld = XMMatrixIdentity();

	matWorld *= matScale;	//スケーリングを反映
	matWorld *= matRot;	//回転を反映
	matWorld *= matTrans;	//平行移動を反映

	if (parent != nullptr) {
		matWorld *= parent->matWorld;
	}

	//定数バッファに転送
	constMap->mat = matWorld * matView * matProjection;
}

void Object3d::Draw()
{
	ID3D12GraphicsCommandList* commandList = directX->GetCommandList();

	// 頂点バッファビューの設定コマンド
	commandList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファビューの設定コマンド
	commandList->IASetIndexBuffer(&ibView);
	//h定数バッファビュー(CBV)の設定コマンド
	commandList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(2, constBuffB1->GetGPUVirtualAddress());

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::descHeap.Get() };
	directX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::descHeap->GetGPUDescriptorHandleForHeapStart();
	UINT incrementSize = directX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvGpuHandle.ptr += incrementSize * textureIndex;
	directX->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// 描画コマンド
	commandList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);

}

void Object3d::CreatePipeline3D()
{
	HRESULT result;

	ID3D12Device* dev = directX->GetDevice();

	ComPtr<ID3DBlob> vsBlob;		// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;		// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob;	// エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/ObjVertexShader.hlsl", // シェーダファイル名
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
		L"Resources/shaders/ObjPixelShader.hlsl", // シェーダファイル名
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
		{//法線ベクトル
		"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		});
	inputLayout.push_back({//uv座標
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		});

	pipeline3D.SetPipeline(vsBlob.Get(), psBlob.Get(), inputLayout);

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
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	//パイプラインにルートシグネチャをセット
	pipeline3D.desc.pRootSignature = rootSignature.Get();

	//パイプラインステートの生成
	pipeline3D.SetPipelineState(dev, pipelineState);
}

void Object3d::LoadMaterial(const std::string& directoryPath, const std::string& filename) {
	//ファイルストリーム
	ifstream file;
	//マテリアルファイルを開く
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	//1行ずつ読み込む
	string line;
	while (getline(file, line)) {

		//1行分の文字列をストリームに変換
		istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭のタブ文字は無視する
		if (key[0] == '\t') {
			key.erase(key.begin());
		}

		//先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl") {
			//マテリアル名読み込み
			line_stream >> material.name;
		}
		//先頭文字列がKaならアンビエント色
		if (key == "Ka") {
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}
		//先頭文字列がKdならディフューズ色
		if (key == "Kd") {
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}
		//先頭文字列がKsならスペキュラー色
		if (key == "Ks") {
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}
		//先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd") {
			//テクスチャのファイル名読み込み
			line_stream >> material.textureFileName;
			//テクスチャ読み込み
			LoadTexture(directoryPath, material.textureFileName);
		}
	}
	//ファイルを閉じる
	file.close();
}

void Object3d::LoadTexture(const std::string& directoryPath, const std::string& filename) {

	//ファイルパスを結合
	string filePath = directoryPath + filename;

	//ユニコード文字列に変換する
	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, wfilepath, _countof(wfilepath));
	
	textureIndex = Texture::LoadTexture(wfilepath);

}