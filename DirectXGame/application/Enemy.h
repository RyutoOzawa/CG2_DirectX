#pragma once
#include"Object3d.h"
#include"SplineCurve.h"
#include"ParticleManager.h"

class Enemy : public Object3d
{
public:

	static void EnemyParticleUpdate();

	static void EnemyInitialize(uint32_t texIndex);

	static void DrawParticle();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="points">�ړ��p�X�v���C���Ȑ��̍��W�z��</param>
	void Initialize(std::vector<Vector3>& points);

	void Update();

	
	void Draw();

	//�X�|�[��
	void Spawn();

	void OnCollision(const CollisionInfo& info) override;

private:

	static ParticleManager particleManager;

	//����
	SplineCurve moveLine;

	bool isAlive = false;



};

