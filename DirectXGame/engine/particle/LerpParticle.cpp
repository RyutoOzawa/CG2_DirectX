#include "LerpParticle.h"
#include"Util.h"

void LerpParticle::Update()
{
	//経過フレームカウント
	frame++;
	//現在時間計算
	float f = (float)frame / num_frame;

	//スケールの補間
	scale = (scaleEnd - scaleStart) * f;
	scale += scaleStart;

	//座標補間
	switch (interType)
	{
	case InterType::Lerp:
		break;
	case InterType::EaseIn:
		//TODO:イージングできるようにする

		break;
	case InterType::EaseOut:
		f = EaseOut(f);

		break;
	default:
		break;
	}

	position = Vector3::Lerp(start, end, f);

	

}

void LerpParticle::Add(int t, const Vector3& start, const Vector3& end, float scaleStart, float scaleEnd, InterType type)
{
	this->start = start;
	this->end = end;
	this->scale = scaleStart;
	this->scaleStart = scaleStart;
	this->scaleEnd = scaleEnd;
	this->num_frame = t;
	this->interType = type;
}
