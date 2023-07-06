#pragma once
#include"Object3d.h"
#include"Model.h"
#include"Vector3.h"

class PlayerBullet : public Object3d
{
public:

	//������
	void Initialize(Model* model,const Vector3& position,const Vector3& velocity);

	void Update();

	bool IsAlive()const { return isAlive; }

private:

	static const int32_t lifeTime = 60 * 5;

	//���x
	Vector3 velocity;
	//�f�X�^�C�}�[
	int32_t deathTimer = lifeTime;
	//�����t���O
	bool isAlive = true;



};

