#pragma once
#include "Object3d.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affine.h"
#include "Input.h"
#include <cassert>
#include "BossBullet.h"
#include<sstream>
#include"Sprite.h"

class BossPhase_1
{

public:// �����o�֐�
	// �������֐�
	void Initialize(SpriteManager* spriteManager);

	// �X�V����
	void Update(Vector3 playerPos);

	/// <summary>
	/// �^�C�g���V�[���`�Q�[���V�[���֑J�ڂ���悤�̍X�V����
	/// </summary>
	void TitleUpdate();

	// �`�揈��
	void Draw(ViewProjection viewprojection);

	// �`�揈��
	void DrawUI();

	WorldTransform GetPos() { return worldTransform_[0]; }

	//�Փ˂����o������Ăяo���ꂽ��R�[���o�b�N�֐�
	void OnCollision();

	std::unique_ptr<BossBullet>& GetBullet() { return bullet; }

	WorldTransform GetWorldTransformP() const { return worldTransform_[0]; }

	int GetHP() { return HP; }

	void Rset();

private:// �ÓI�����o�֐�

	// �u���b�N���΂�����
	void FlyBlocks(Vector3 playerPos);

	// �u���b�N�������_���ɒ��I���ĕ������鏈��
	void FloatRandomBlocks();

	// �e��ł֘A�̏�����
	void ResetFlyBlocks();

	// �s��̓]���ƍX�V
	void TransferMat();

	void TurnBodyToPlayer(Vector3 playerPos);
private:// �����o�ϐ�

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_[27];

	// �C���v�b�g
	Input* input_ = nullptr;

	//���f��
	Object3d* model_ = nullptr;

	//�f�X�t���O
	bool isDead_ = false;

	// �����Ƃ��̃t���O
	bool AnnihilationFlag[27];

	// �����_���ɂƂ邽�߂̕ϐ�
	int randomBlock = 0;

	// �u���b�N�𕂂����I���܂ł̃t���O
	bool FloatBlockFlagM = false; // ���W���}�C�i�X
	bool FloatBlockFlagP = false; // ���W���v���X
	
	// �����o���Ƃ��̊p�x
	Vector3 rotaAngle;

	// �u���b�N�𓮂����O�̈ʒu
	Vector3 oldPos;

	// �{�X�o���b�g
	std::unique_ptr<BossBullet> bullet;

	// ���X�Ɋg�傷�鎞�̃X�P�[��
	Vector3 expansionScale;

	// ���X�Ɋg�傷��Ƃ��̃X�s�[�h
	Vector3 expansionScaleSpeed = { 0.05f,0.05f,0.05f };

	//�^�C�g�������p��]�p
	float titleRadian = 0;

	const int maxHP = 10;
	int HP = maxHP;

	Sprite* spriteHP;
	Sprite* spriteHPBar;
};

