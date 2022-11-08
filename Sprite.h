#pragma once

#include"SpriteManager.h"
#include<DirectXMath.h>


class Sprite
{
	//�}�l�[�W���N���X�̃|�C���^
	SpriteManager* spriteManager = nullptr;
public:
	DirectX::XMFLOAT3 vertices[4] = {};
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
public:
	void Initialize(SpriteManager* spriteManager);

	void Draw();
};

