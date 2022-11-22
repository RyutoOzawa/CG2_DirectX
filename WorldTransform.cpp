#include "WorldTransform.h"
#include<cassert>
#include<DirectXMath.h>
using namespace DirectX;

ReDirectX* WorldTransform::directX = nullptr;

void WorldTransform::StaticInitialize(ReDirectX* directX_)
{
	//NULLチェック
	assert(directX_);

	//メンバにコピー
	directX = directX_;
}

void WorldTransform::Initialise()
{
	CreateConstBuffer();
	Map();
	TransferMatrix();
}

void WorldTransform::CreateConstBuffer()
{
	HRESULT result;
	ID3D12Device* device = directX->GetDevice();
	D3D12_HEAP_PROPERTIES cbHeapProp{  };
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataWorldTransform) + 0xff) & ~0xff;	//256バイトアラインメント
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
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
}

void WorldTransform::Map()
{
	//定数バッファのマッピング
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);//マッピング
	assert(SUCCEEDED(result));
}

void WorldTransform::TransferMatrix()
{
	Matrix4 matScale, matRot, matTrans;

	//各行列計算
	matScale = matScale.identity();
	matRot = matRot.identity();
	matTrans = matTrans.identity();
	matWorld = matWorld.identity();

	matScale = matScale.scale(scale);
	matRot *= matRot.rotateZ(rotation.z);
	matRot *= matRot.rotateX(rotation.x);
	matRot *= matRot.rotateY(rotation.y);
	matTrans = matTrans.translate(translation);

	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//定数バッファに転送
	constMap->matWorld = matWorld;

}
