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

	//更新
	void Update();

	//開始
	void Start(float allFrame);
	
	//getter
	float GetTimeRate() { return timeRate; }

};

//イージング
float Out(float t);
float In(float t);

float InOut(float t);
float OutBounce(float t);