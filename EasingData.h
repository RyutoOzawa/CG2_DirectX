#pragma once
#include<time.h>

class EasingData
{
private:

	float maxTime = 0;
	float nowTime = 0;
	float timeRate = 0;

public:

	void Update();

	void Start(float allFrame);

	float GetTimeRate() { return timeRate; }

};

