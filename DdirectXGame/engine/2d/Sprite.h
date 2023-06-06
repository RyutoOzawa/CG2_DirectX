#pragma once


#include<DirectXMath.h>
#include"Material.h"
#include"Texture.h"
#include<string>
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4.h"

enum VertexNumber {
	LB,	//����
	LT,	//����
	RB,	//�E��
	RT,	//�E��
};

class Sprite
{
public://�T�u�N���X
	struct VertexPosUv {
		Vector3 pos;	//xyz���W
		Vector2 uv;	//uv���W
	};

	struct ConstBufferData {
		Vector4 color;
		Matrix4 mat;
	};

protected://�ÓI�����o�ϐ�

	static std::string defaultTextureDirectoryPath;//�f�t�H���g�̃f�B���N�g���p�X
	static ID3D12Device* device;//�f�o�C�X
	static ID3D12GraphicsCommandList* cmdList;//�R�}���h���X�g	
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;		//�p�C�v���C���X�e�[�g
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;		//���[�g�V�O�l�`��

	static Matrix4 matProjection;	//�ˉe�s��

public://�ÓI�����o�֐�
	static void StaticInitialize(ID3D12Device* device_, ID3D12GraphicsCommandList* cmdList_, int windowWidth, int windowHeight);

	static void BeginDraw();

	static void SetTextureCommand(uint32_t index);

	//�X�v���C�g�p�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐���
	static void CreatePipeline2D();

protected:	//�����o�ϐ�

	//�}�l�[�W���N���X�̃|�C���^
	VertexPosUv vertices[4] = {};		//���_���W�f�[�^
	D3D12_VERTEX_BUFFER_VIEW vbView{};	//���_�o�b�t�@�r���[
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;	//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff = nullptr;	//�萔�o�b�t�@
	uint32_t textureIndex = 0;	//�e�N�X�`���Ɋ��蓖�Ă��Ă���ԍ�
	ConstBufferData* constMap = nullptr;//�萔�o�b�t�@�\����

	Matrix4 matWorld{};	//���[���h�ϊ��s��
	float rotation = 0.0f;	//��]�p
	Vector2 position = { 0.0f,0.0f };	//���W
	Vector4 color = { 1,1,1,1 };	//���W
	Vector2 size = { 100.0f,100.0f };//�X�v���C�g�T�C�Y
	Vector2 anchorPoint = { 0.0f,0.0f };//�A���J�[�|�C���g(���W�ϊ��̊�_)
	bool isFlipX = false;	//���E���]�t���O
	bool isFlipY = false;	//�㉺���]�t���O
	bool isInvisible = false;	//��\���t���O

	Vector2 textureLeftTop = { 0.0f,0.0f };	//�e�N�X�`��������W
	Vector2 textureSize = { 100.0f,100.0f };	//�e�N�X�`���؂�o���T�C�Y

public: //�����o�֐�

	Sprite();
	Sprite(uint32_t texIdnex,Vector2 pos,Vector2 size_,Vector4 color_,Vector2 anchorP,bool isFlipX_,bool isFlipY_);

	void Initialize(uint32_t textureNum = UINT32_MAX);

	void Draw();
	void SetColor(const Vector4& color_) { color = color_; }
	void SetPos(const Vector2& pos) { position = pos; }
	void SetRotation(float rotation) { this->rotation = rotation; }
	void SetSize(const Vector2& size_) { size = size_; }
	void SetAnchorPoint(const Vector2& point) { anchorPoint = point; }
	void SetFlipX(bool flipX) { isFlipX = flipX; }
	void SetFlipY(bool flipY) { isFlipY = flipY; }
	void SetInvisible(bool flag) { isInvisible = flag; }
	void SetTextureNum(uint32_t index) { textureIndex = index; }
	void SetTextureLeftTop(const Vector2& leftTop) { textureLeftTop = leftTop; }
	void SetTextureSize(const Vector2& size) { textureSize = size; }

	const Vector2& GetPosition()const { return position; }
	float GetRotation()const { return rotation; }
	const Vector4 GetColor()const { return color; }
	const Vector2 GetSize()const { return size; }
	const Vector2 GetAnchorPoint()const { return anchorPoint; }
	bool GetIsFlipX()const { return isFlipX; }
	bool GetIsFlipY()const { return isFlipY; }
	bool GetIsInvisible()const { return isInvisible; }
	uint32_t GetTextureNum()const { return textureIndex; }
	const Vector2 GetTextureLeftTop()const { return textureLeftTop; }
	const Vector2 GetTextureSize()const { return textureSize; }

	void Update();

private:
	//�e�N�X�`���T�C�Y���C���[�W�T�C�Y�ɍ��킹��
	void AdjustTextureSize();


};

