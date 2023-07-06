#include "EasingData.h"
#include<cmath>

void EasingData::Update()
{
	//現在時間を加算
	nowTime += 1.0f;

	//進行度÷最大時間で進み具合を計算
	timeRate = nowTime / maxTime;
	if (timeRate > 1.0f) {
		timeRate = 1.0f;
	}

}

void EasingData::Start(float allFrame)
{
	//現在時間を初期化
	nowTime = 0.0f;
	//最大フレームのセット
	maxTime = allFrame;
}
