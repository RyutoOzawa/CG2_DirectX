//�C�[�W���O�f�[�^�N���X1
//�C�[�W���O�Ŏg�����Ԃ̊Ǘ���J�n�����s��

#include "EasingData.h"
#include<cmath>

void EasingData::Update()
{
	if (!isActive) {
		return;
}

	//���ݎ��Ԃ����Z
	nowTime += 1.0f;

	//�i�s�x���ő厞�ԂŐi�݋���v�Z
	timeRate = nowTime / maxTime;
	if (timeRate > 1.0f) {
		timeRate = 1.0f;
		isActive = false;
	}

}

void EasingData::Start(float allFrame)
{
	isActive = true;

	//���ݎ��Ԃ�������
	nowTime = 0.0f;
	//�ő�t���[���̃Z�b�g
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

float OutBounce(float t)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (t < 1 / d1) {
		return n1 * t * t;
	}
	else if (t < 2 / d1) {
		return n1 * (t -= 1.5f / d1) * t + 0.75f;
	}
	else if (t < 2.5f / d1) {
		return n1 * (t -= 2.25f / d1) * t + 0.9375f;
	}
	else {
		return n1 * (t -= 2.625f / d1) * t + 0.984375f;
	}
}
