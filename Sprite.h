#pragma once

#include"SpriteManager.h"
#include<DirectXMath.h>


class Sprite
{
	//�}�l�[�W���N���X�̃|�C���^
	SpriteManager* spriteManager = nullptr;
public:
	DirectX::XMFLOAT3 vertices[3] = {};
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
public:
	void Initialize(SpriteManager* spriteManager);

	void Draw();
};

