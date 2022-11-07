#pragma once

#include"SpriteManager.h"
#include<DirectXMath.h>


class Sprite
{
	//マネージャクラスのポインタ
	SpriteManager* spriteManager = nullptr;
public:
	DirectX::XMFLOAT3 vertices[3] = {};
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
public:
	void Initialize(SpriteManager* spriteManager);

	void Draw();
};

