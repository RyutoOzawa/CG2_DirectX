#include "EasingData.h"
#include<cmath>

float EasingData::Update()
{
	//現在時刻を取得
	nowCount = clock();
	//start()で取得した開始時間との差分
	endCount = nowCount - startCount;
	//進行度をfloatに変換
	endTime = (float)endCount / 1'000.0f;
	//進行度÷最大時間で進み具合を計算
	timeRate = endTime / maxTime;
	if (timeRate > 1.0f) {
		timeRate = 1.0f;
	}

	return timeRate;
}

void EasingData::Start(float time)
{
	startCount = clock();
	maxTime = time;
}
