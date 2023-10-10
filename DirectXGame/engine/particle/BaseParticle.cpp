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

void BaseParticle::Add(int life, const Vector3& position_, const Vector3& velocity_,
	const Vector3& accel_, float scaleStart_, float scaleEnd_)
{
	position = position_;
	velocity = velocity_;
	accel = accel_;
	num_frame = life;
	scale = scaleStart_;
	scaleStart = scaleStart_;
	scaleEnd = scaleEnd_;
}
