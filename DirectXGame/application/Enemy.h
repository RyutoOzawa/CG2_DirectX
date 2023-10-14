#pragma once
//�G�N���X
//�Q�[���ɓo�ꂷ��G�P�̂̏������s��

#include"Object3d.h"
#include"SplineCurve.h"
#include"ParticleManager.h"
#include"EnemyBullet.h"

/// <summary>
/// �G�̔������
/// </summary>
struct EnemyData {
	float spawnTime = 0.0f;

};

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

	//����
	SplineCurve moveLine;

	bool isAlive = false;

	//�e�֌W
	INT32 shotInterval = 0;
	static const INT32 shotCoolTime;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	static Model* model;
	static Model* bulletModel;

	//�傫���̐���
	void ScaleControll();

	//�X�s��
	void Spin();

	//�U������]�̃C�[�W���O�f�[�^
	EasingData easeAtkRot;

	Vector3 baseScale = { 3,3,3 };
};

