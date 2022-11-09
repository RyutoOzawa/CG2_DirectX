#pragma once

#include"SpriteManager.h"
#include<DirectXMath.h>
#include"Material.h"
#include"Texture.h"
#include<string>
#include"Vector3.h"

class Sprite
{
	//マネージャクラスのポインタ
	SpriteManager* spriteManager = nullptr;

public:	//メンバ変数
	VertexPosUv vertices[4] = {};		//頂点座標データ
	D3D12_VERTEX_BUFFER_VIEW vbView{};	//頂点バッファビュー
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff = nullptr;	//定数バッファ
	Texture texture;	//画像データ
	UINT texNum;
	//定数バッファ構造体
	ConstBufferData* constMap = nullptr;

	DirectX::XMMATRIX matWorld;	//ワールド変換行列
	DirectX::XMFLOAT3 rotation;	//回転角
	DirectX::XMFLOAT2 position = {0.0f,0.0f};	//座標


public: //メンバ関数
	void Initialize(SpriteManager* spriteManager, const wchar_t filename[]);

	void Draw();

	void SetColor(DirectX::XMFLOAT4 color_);
	void SetPos(const DirectX::XMFLOAT2& pos) { position = pos; }
	//void SetRotation(float rotation);

	const DirectX::XMFLOAT2& GetPosition()const { return position; }


};

