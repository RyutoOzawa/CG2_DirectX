#pragma once
#include "DirectX.h"
#include "Input.h"
#include "Object3d.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include<cassert>
#include"playerBullet.h"
#include<memory>
#include<list>
#include<cmath>
#include"Audio.h"
class player
{
public:
	~player();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(SpriteManager *spriteManager);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	///<summary>
	///�U��
	///</summary>
	void Attack();

	///<summary>
	///�W�����v
	///</summary>
	void jump();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection_);

	/// <summary>
	/// UI�`��
	/// </summary>
	void DrawUI();

	//�Փ˂����o������Ăяo���ꂽ��R�[���o�b�N�֐�
	void OnCollision();

	//���[���h���W���擾
	Vector3 GetworldPosition();

	const std::list<std::unique_ptr<playerBullet>>& GetBullets() { return bullets_; }

	const WorldTransform* GetWorldTransform() { return &worldTransform_; }

	void SetEndMoveRotation(Vector3 Transform);

	void AllBulletDelete() { bullets_.clear(); }

	void TransformRset(bool endOrStart);

	void Rset();

	int GetHP() { return HP; }
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Object3d* model_ = nullptr;
	Object3d* bulletModel_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	//�e
	std::list<std::unique_ptr<playerBullet>>bullets_;

	float PI = 3.1415927f;

	//3D���e�B�N���p���[���h�g�����X�t�H�[��
	WorldTransform worldTransform3DReticle_;

	//2D���e�B�N���p�X�v���C�g
	std::unique_ptr<Sprite> sprite2DReticle_;

	uint8_t jumpFlag = 0u;

	const float defGravitySpeed = 1.5;
	float gravitySpeed = defGravitySpeed;

	const int maxHP = 3;
	int HP = maxHP;

	bool isDamageInterval;

	int damageInterval;

	int attackInterval = 0;
	const int attackDelay = 30;

	Sprite* spriteHP[3];

	Sound AttackSound;
	Sound damageSound;
	Sound deadSound;
	Sound janpSound;
};

