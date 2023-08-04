#include "BaseParticle.h"

void BaseParticle::Update()
{
	//経過フレームカウント
	frame++;
	//加速
	velocity += accel;
	//移動
	position += velocity;
	//現在時間計算
	float f = (float)frame / num_frame;
	//スケールの補間
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
