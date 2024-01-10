//イージングデータクラス1
//イージングで使う時間の管理や開始等を行う

#include "EasingData.h"
using namespace Easing;
#include<cmath>

void EasingData::Update()
{
	if (!isActive) {
		return;
}

	//現在時間を加算
	nowTime += 1.0f;

	//進行度÷最大時間で進み具合を計算
	timeRate = nowTime / maxTime;
	if (timeRate > 1.0f) {
		timeRate = 1.0f;
		isActive = false;
	}

}

void EasingData::Start(float allFrame)
{
	isActive = true;

	//現在時間を初期化
	nowTime = 0.0f;
	//最大フレームのセット
	maxTime = allFrame;
}

float Easing::Out(float t)
{
	return 1 - powf(1 - t, 3);
}

float Easing::In(float t)
{
	return t * t * t;
}

float Easing::InOut(float t)
{
	return t < 0.5f ? 4 * t * t * t : 1 - powf(-2 * t + 2, 3) / 2.0f;
}

float Easing::OutBounce(float t)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (t < 1 / d1) {
		return n1 * t * t;
	}
	else if (t < 2 / d1) {
		return n1 * (t -= 1.5f / d1) * t + 0.75f;
	}
	else if (t < 2.5f / d1) {
		return n1 * (t -= 2.25f / d1) * t + 0.9375f;
	}
	else {
		return n1 * (t -= 2.625f / d1) * t + 0.984375f;
	}
}

float Easing::InBounce(float t)
{
	return 1 -OutBounce(1 - t);
}
