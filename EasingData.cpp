#include "EasingData.h"
#include<cmath>

void EasingData::Update()
{
	//���ݎ��Ԃ����Z
	nowTime += 1.0f;

	//�i�s�x���ő厞�ԂŐi�݋���v�Z
	timeRate = nowTime / maxTime;
	if (timeRate > 1.0f) {
		timeRate = 1.0f;
	}

}

void EasingData::Start(float allFrame)
{
	//���ݎ��Ԃ�������
	nowTime = 0.0f;
	//�ő�t���[���̃Z�b�g
	maxTime = allFrame;
}
