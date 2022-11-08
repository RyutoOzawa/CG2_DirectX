#pragma once

#include"SpriteManager.h"
#include<DirectXMath.h>
#include"Material.h"

class Sprite
{
	//�}�l�[�W���N���X�̃|�C���^
	SpriteManager* spriteManager = nullptr;
public:
	VertexPosUv vertices[4] = {};		//���_���W�f�[�^
	D3D12_VERTEX_BUFFER_VIEW vbView{};	//���_�o�b�t�@�r���[
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;	//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff = nullptr;	//�萔�o�b�t�@

	ConstBufferDataMaterial* constMapMaterial = nullptr;	//�F�f�[�^
public:
	void Initialize(SpriteManager* spriteManager);

	void Draw();

	void SetColor(DirectX::XMFLOAT4 color_);
};

