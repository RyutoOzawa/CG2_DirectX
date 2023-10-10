#pragma once
#include"Vector3.h"

class BaseParticle
{

protected:

	//座標
	Vector3 position;
	//現在スケール
	float scale = 1.0f;
	//現在フレーム
	int frame = 0;
	//終了フレーム
	int num_frame = 0;

public:
	virtual void Update();

	virtual void Add(int life, const Vector3& position_, const Vector3& velocity_,
		const Vector3& accel_, float scaleStart_, float scaleEnd_);

	bool IsAlive() { return frame > num_frame; }

	Vector3 GetPosition()const { return position; }

	float GetScale()const { return scale; }

private:
	//スケール開始値
	float scaleStart = 1.0f;
	//スケール終了値
	float scaleEnd = 0.0f;
	//速度
	Vector3 velocity = {};
	//加速度
	Vector3 accel = {};
};

