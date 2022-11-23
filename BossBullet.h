#pragma once
#include "Object3d.h"
#include "WorldTransform.h"
#include <cassert>
#include "affine.h"


class BossBullet
{
	~BossBullet();
public:
	// ������
	void Initialize(Object3d* model, const WorldTransform worldTransform,const Vector3& OyaPos, const Vector3& rotaAngle);

	// �X�V����
	void Update(const Vector3& playerPos);

	// �`�揈��
	void Draw(const ViewProjection& viewProjection);

	// �f�X�t���O�̃Q�b�^�[
	bool IsDead() const { return isDead_; }

	// ���[���h�e���W���擾
	Vector3 GetBulletWorldPosition();

	// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	// �u���b�N���΂�����
	void FlyBlocks(Vector3 playerPos);

	// ���Z�b�g
	void Reset();

	// �v���C���[�̈ʒu�ɍs�������̃t���O�̃Q�b�^�[
	bool GetToPlayerFlag() const { return ToPlayerFlag_; }

	void SetToPlayerFlag(bool ToPlayerFlag);

	inline void SetPos(const Vector3& pos) {worldTransform_.translation_ = pos;}

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Object3d* model_ = nullptr;

	//���x
	Vector3 velocity_;

	//����<frm>
	static const int32_t kLifeTime = 60 * 6;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = 0;

	//�f�X�t���O
	bool isDead_ = false;

	// �u���b�N�𕂂����I���܂ł̃t���O
	bool FloatBlockFlagM = false; // ���W���}�C�i�X
	bool FloatBlockFlagP = false; // ���W���v���X
	bool FloatXRimitFlag = false; // ���ɏo���Ƃ��̐����t���O]


	// ��΂����u���b�N�v���C���[�Ɍ������ĉ�]����t���O
	bool flyToPlayerFlag = false;

	// �u���b�N�𓮂����O�̈ʒu
	Vector3 oldPos;
	Vector3 oyaPos;
	Vector3 playerPos;


	// �v���C���[�̏��Ɏ��������̃t���O
	bool ToPlayerFlag_ = false;
};

