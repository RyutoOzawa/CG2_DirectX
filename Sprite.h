#pragma once

#include"SpriteManager.h"
#include<DirectXMath.h>


class Sprite
{
	//マネージャクラスのポインタ
	SpriteManager* spriteManager = nullptr;
public:
	DirectX::XMFLOAT3 vertices[4] = {};
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
public:
	void Initialize(SpriteManager* spriteManager);

	void Draw();
};

