#pragma once

#include"SpriteManager.h"
#include<DirectXMath.h>
#include"Material.h"
#include"Texture.h"
#include<string>
#include"Vector3.h"

enum VertexNumber {
	LB,	//����
	LT,	//����
	RB,	//�E��
	RT,	//�E��
};

class Sprite
{
	//�}�l�[�W���N���X�̃|�C���^
	SpriteManager* spriteManager = nullptr;

public:	//�����o�ϐ�

	VertexPosUv vertices[4] = {};		//���_���W�f�[�^
	D3D12_VERTEX_BUFFER_VIEW vbView{};	//���_�o�b�t�@�r���[
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;	//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff = nullptr;	//�萔�o�b�t�@
	//Texture texture;	//�摜�f�[�^
	uint32_t textureIndex = 0;
	//�萔�o�b�t�@�\����
	ConstBufferData* constMap = nullptr;

	DirectX::XMMATRIX matWorld;	//���[���h�ϊ��s��
	float rotation;	//��]�p
	DirectX::XMFLOAT2 position = { 0.0f,0.0f };	//���W
	DirectX::XMFLOAT4 color = { 1,1,1,1 };	//���W
	DirectX::XMFLOAT2 size = { 100.0f,100.0f };//�X�v���C�g�T�C�Y
	DirectX::XMFLOAT2 anchorPoint = { 0.0f,0.0f };//�A���J�[�|�C���g(���W�ϊ��̊�_)
	bool isFlipX = false;	//���E���]�t���O
	bool isFlipY = false;	//�㉺���]�t���O
	bool isInvisible = false;	//��\���t���O

public: //�����o�֐�
	void Initialize(SpriteManager* spriteManager);

	void Draw();
	void SetColor(const DirectX::XMFLOAT4& color_) { color = color_; }
	void SetPos(const DirectX::XMFLOAT2& pos) { position = pos; }
	void SetRotation(float rotation) { this->rotation = rotation; }
	void SetSize(const DirectX::XMFLOAT2& size_) { size = size_; }
	void SetAnchorPoint(const DirectX::XMFLOAT2& point) { anchorPoint = point; }
	void SetFlipX(bool flipX) { isFlipX = flipX; }
	void SetFlipY(bool flipY) { isFlipY = flipY; }
	void SetInvisible(bool flag) { isInvisible = flag; }
	void SetTextureNum(uint32_t index) { textureIndex = index; }

	const DirectX::XMFLOAT2& GetPosition()const { return position; }
	float GetRotation()const { return rotation; }
	const DirectX::XMFLOAT4 GetColor()const { return color; }
	const DirectX::XMFLOAT2 GetSize()const { return size; }
	const DirectX::XMFLOAT2 GetAnchorPoint()const { return anchorPoint; }
	bool GetIsFlipX()const { return isFlipX; }
	bool GetIsFlipY()const { return isFlipY; }
	bool GetIsInvisible()const { return isInvisible; }
	uint32_t GetTextureNum()const { return textureIndex; }

	void Update();
};

