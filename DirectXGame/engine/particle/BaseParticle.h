#pragma once
//基本パーティクルクラス
//マネージャによって生成されるパーティクルのパラメータ等を持つ

#include"Vector3.h"
#include"Vector4.h"

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
	//色
	Vector4 color = { 1,1,1,1 };

public:
	//更新
	virtual void Update();

	/// <summary>
	/// パーティクル追加
	/// </summary>
	/// <param name="life">生存時間</param>
	/// <param name="position_">座標</param>
	/// <param name="velocity_">速度</param>
	/// <param name="accel_">加速度</param>
	/// <param name="scaleStart_">開始スケール</param>
	/// <param name="scaleEnd_">終了スケール</param>
	virtual void Add(int life, const Vector3& position_, const Vector3& velocity_,
		const Vector3& accel_, float scaleStart_, float scaleEnd_, Vector4 color_ ={1,1,1,1});

	//生存フラグ
	bool IsAlive() { return frame > num_frame; }

	//getter
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

