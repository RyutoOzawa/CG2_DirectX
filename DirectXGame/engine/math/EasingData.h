#pragma once
//イージングデータクラス1
//イージングで使う時間の管理や開始等を行う

class EasingData
{
private:

	float maxTime = 0;
	float nowTime = 0;
	float timeRate = 0;
	bool isActive = false;

public:

	void Update();

	void Start(float allFrame);

	float GetTimeRate() { return timeRate; }

};

float Out(float t);

float In(float t);

float InOut(float t);