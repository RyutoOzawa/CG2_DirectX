#include "BaseParticle.h"

void BaseParticle::Update()
{
	//�o�߃t���[���J�E���g
	frame++;
	//����
	velocity += accel;
	//�ړ�
	position += velocity;
	//���ݎ��Ԍv�Z
	float f = (float)frame / num_frame;
	//�X�P�[���̕��
	scale = (scaleEnd - scaleStart) * f;
	scale += scaleStart;

}

void BaseParticle::Add(int life, const Vector3& position, const Vector3& velocity, const Vector3& accel, float scaleStart, float scaleEnd)
{
	this->position = position;
	this->velocity = velocity;
	this->accel = accel;
	this->num_frame = life;
	this->scale = scaleStart;
	this->scaleStart = scaleStart;
	this->scaleEnd = scaleEnd;
}
