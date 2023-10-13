#pragma once
#include"BaseParticle.h"

enum class InterType {
	Lerp,		//線形補間
	EaseIn,		//イージング(遅→早)
	EaseOut,	//イージング(早→遅)
};

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
	InterType interType = InterType::Lerp;

public:
	void Update() override;

	void Add(int t, const Vector3& start_, const Vector3& end_, float scaleStart_, float scaleEnd_, InterType type);
};

