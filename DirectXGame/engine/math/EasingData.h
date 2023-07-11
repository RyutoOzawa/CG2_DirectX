#pragma once

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

