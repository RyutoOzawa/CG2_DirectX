#pragma once
//線形補間パーティクルクラス　
//基本のパーティクルクラスを継承し、パーティクルの移動を線形補間で行うクラス

#include"BaseParticle.h"
#include"EasingData.h"



//パーティクル基盤クラスを継承
class LerpParticle : public BaseParticle
{
private:

	//始点
	Vector3 start = {};
	//終点
	Vector3 end = {};
	//スケール開始値
	float scaleStart = 1.0f;
	//スケール終了値
	float scaleEnd = 0.0f;

	//補間の種類
	Easing::InterType interType = Easing::InterType::Lerp;

public:
	//更新
	void Update() override;

	/// <summary>
	/// パーティクル追加
	/// </summary>
	/// <param name="t">生存時間</param>
	/// <param name="start_">始点</param>
	/// <param name="end_">終点</param>
	/// <param name="scaleStart_">開始スケール</param>
	/// <param name="scaleEnd_">終了スケール</param>
	/// <param name="type">補間の種類</param>
	void Add(int t, const Vector3& start_, const Vector3& end_, float scaleStart_, float scaleEnd_, 
		Easing::InterType type = Easing::InterType::Lerp, Vector4 color_ = {1,1,1,1});
};

