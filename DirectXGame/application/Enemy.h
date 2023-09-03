#pragma once
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

	static void EnemyParticleUpdate();

	static void EnemyInitialize(TextureData* texData, Model* enemyModel,Model* bulletModel);

	static void DrawParticle();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="points">�ړ��p�X�v���C���Ȑ��̍��W�z��</param>
	void Initialize(std::vector<Vector3>& points);

	void Update(const Vector3& playerWorldPos,const Matrix4& cameraMat);

	
	void Draw();

	//�X�|�[��
	void Spawn();

	void OnCollision(const CollisionInfo& info) override;

	bool IsAlive()const { return isAlive; }

	void Attack(const Vector3& playerWorldPos);

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

	Vector3 baseScale = { 3,3,3 };
};

