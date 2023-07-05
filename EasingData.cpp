#include "EasingData.h"
#include<cmath>

float EasingData::Update()
{
	//���ݎ������擾
	nowCount = clock();
	//start()�Ŏ擾�����J�n���ԂƂ̍���
	endCount = nowCount - startCount;
	//�i�s�x��float�ɕϊ�
	endTime = (float)endCount / 1'000.0f;
	//�i�s�x���ő厞�ԂŐi�݋���v�Z
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
