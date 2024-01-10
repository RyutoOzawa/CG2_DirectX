//線形補間パーティクルクラス　
//基本のパーティクルクラスを継承し、パーティクルの移動を線形補間で行うクラス

#include "LerpParticle.h"
#include"Util.h"
using namespace Utility;
using namespace Easing;

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
		f = Out(f);

		break;
	default:
		break;
	}

	position = Vector3::Lerp(start, end, f);

	

}

void LerpParticle::Add(int t, const Vector3& start_, const Vector3& end_, float scaleStart_, float scaleEnd_, InterType type,Vector4 color_)
{
	start = start_;
	end = end_;
	scale = scaleStart_;
	scaleStart = scaleStart_;
	scaleEnd = scaleEnd_;
	num_frame = t;
	interType = type;
	color = color_;
}
