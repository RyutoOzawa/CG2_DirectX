#pragma once
//�G�N���X
//�Q�[���ɓo�ꂷ��G�P�̂̏������s��

#include"Object3d.h"
#include"SplineCurve.h"
#include"ParticleManager.h"
#include"EnemyBullet.h"


class Enemy : public Object3d
{
public:

	//�p�[�e�B�N���X�V
	static void EnemyParticleUpdate();

	//�G�̐ÓI������
	static void EnemyInitialize(TextureData* texData, Model* enemyModel,Model* bulletModel_);

	//�p�[�e�B�N���`��
	static void DrawParticle();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="points">�ړ��p�X�v���C���Ȑ��̍��W�z��</param>
	void Initialize(std::vector<Vector3>& points);

	/// <summary>
	/// ������(�t�@�C���ǂݍ��ݔ�)
	/// </summary>
	/// <param name="spawnPos">���W</param>
	/// <param name="leaveTime">�ޔ��܂ł̃t���[��</param>
	void Initialive(const Vector3& spawnPos, uint16_t leaveTime_);


	//�X�V
	void Update(const Vector3& playerWorldPos,const Matrix4& cameraMat);

	//�`��
	void Draw();

	//�X�|�[��
	void Spawn();
	//�����蔻��R�[���o�b�N
	void OnCollision([[maybe_unused]] const CollisionInfo& info) override;
	//�����t���O
	bool IsAlive()const { return isAlive; }
	//�U��
	void Attack(const Vector3& playerWorldPos);
	//���S
	void Death();

private:

	static ParticleManager particleManager;

	const float enemyColSize = 9.0f;

	//����
	SplineCurve moveLine;
	uint16_t leaveTime = 0;
	Vector3 stayPosition = {};
	Vector3 leaveSpd = {};
	const float leaveSpdBase = 5.0f;

	bool isAlive = false;

	//�e�֌W
	INT32 shotInterval = 0;
	static const INT32 shotCoolTime;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	static Model* model;
	static Model* bulletModel;

	//�ړ�
	void Move(const Matrix4& camMat,const Vector3& camPos);

	//�ޔ�
	void Leave();

	//�傫���̐���
	void ScaleControll();

	//�X�s��
	void Spin();

	//�U������]�̃C�[�W���O�f�[�^
	Easing::EasingData easeAtkRot;

	Vector3 baseScale = { 3,3,3 };
};

