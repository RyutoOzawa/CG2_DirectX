#include "EasingData.h"
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

float Out(float t)
{
	return 1 - powf(1 - t, 3);
}

float In(float t)
{
	return t * t * t;
}

float InOut(float t)
{
	return t < 0.5f ? 4 * t * t * t : 1 - powf(-2 * t + 2, 3) / 2.0f;
}
