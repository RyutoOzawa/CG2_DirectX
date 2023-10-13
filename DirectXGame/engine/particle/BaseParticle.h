#pragma once
#include"Vector3.h"

class BaseParticle
{

protected:

	//���W
	Vector3 position;
	//���݃X�P�[��
	float scale = 1.0f;
	//���݃t���[��
	int frame = 0;
	//�I���t���[��
	int num_frame = 0;

public:
	virtual void Update();

	virtual void Add(int life, const Vector3& position_, const Vector3& velocity_,
		const Vector3& accel_, float scaleStart_, float scaleEnd_);

	bool IsAlive() { return frame > num_frame; }

	Vector3 GetPosition()const { return position; }

	float GetScale()const { return scale; }

private:
	//�X�P�[���J�n�l
	float scaleStart = 1.0f;
	//�X�P�[���I���l
	float scaleEnd = 0.0f;
	//���x
	Vector3 velocity = {};
	//�����x
	Vector3 accel = {};
};

