#pragma once

#include"SpriteManager.h"
#include<DirectXMath.h>
#include"Material.h"
#include"Texture.h"
#include<string>

class Sprite
{
	//マネージャクラスのポインタ
	SpriteManager* spriteManager = nullptr;
public:
	VertexPosUv vertices[4] = {};		//頂点座標データ
	D3D12_VERTEX_BUFFER_VIEW vbView{};	//頂点バッファビュー
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff = nullptr;	//定数バッファ
	Texture texture;	//画像データ
	UINT texNum;

	ConstBufferDataMaterial* constMapMaterial = nullptr;	//色データ
public:
	void Initialize(SpriteManager* spriteManager, const wchar_t filename[]);

	void Draw();

	void SetColor(DirectX::XMFLOAT4 color_);
};

