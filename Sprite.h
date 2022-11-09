#pragma once

#include"SpriteManager.h"
#include<DirectXMath.h>
#include"Material.h"
#include"Texture.h"
#include<string>

class Sprite
{
	//�}�l�[�W���N���X�̃|�C���^
	SpriteManager* spriteManager = nullptr;

public:	//�����o�ϐ�
	VertexPosUv vertices[4] = {};		//���_���W�f�[�^
	D3D12_VERTEX_BUFFER_VIEW vbView{};	//���_�o�b�t�@�r���[
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;	//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff = nullptr;	//�萔�o�b�t�@
	Texture texture;	//�摜�f�[�^
	UINT texNum;

	ConstBufferDataMaterial* constMapMaterial = nullptr;	//�F�f�[�^

	//�萔�o�b�t�@�\����
	ConstBufferData* constMap = nullptr;

public: //�����o�֐�
	void Initialize(SpriteManager* spriteManager, const wchar_t filename[]);

	void Draw();

	void SetColor(DirectX::XMFLOAT4 color_);

private:
	void CreateConstBuff();
};

