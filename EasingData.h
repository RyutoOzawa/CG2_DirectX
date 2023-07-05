#pragma once
#include<time.h>

class EasingData
{
private:
	static long long nowTime;

	long long nowCount = 0;
	long long startCount = 0;
	long long endCount = 0;

	float maxTime = 0;
	float endTime = 0;
	float timeRate = 0;

public:

	float Update();

	void Start(float time);

	float GetTimeRate() { return Update(); }

};

