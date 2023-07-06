#include "SplineCurve.h"

void SplineCurve::Start(float allTime)
{
	//��Ԃ��s����
	int32_t lerpCount;

	//��Ԃ̉񐔂͔z��̑���-2
	lerpCount = (int32_t)controllPoints.size()-2;

	//��Ԃ̑S�̎��ԁ���Ԃ̉񐔂ň��ԓ�����̎��Ԃ��v�Z
	allTimeOnce = allTime / (float)lerpCount;

	//�_�~�[���W�̃Z�b�g
	dummyPoints[0] = controllPoints.front();
	dummyPoints[1] = controllPoints.back();

	currentPos = controllPoints[0];

	//�J�n���̗v�f�ԍ������������ăC�[�W���O�J�n
	startIndex = 0;
	eData.Start(allTimeOnce);


}

void SplineCurve::Update()
{
	//�C�[�W���O�f�[�^�̍X�V
	eData.Update();

	float t = eData.GetTimeRate();
	//t��1�ȏ�ɂȂ�������Ԃ̕�ԏI��
	if (t >= 1.0f) {
		//�ēx�C�[�W���O�J�n����startindex��1�i�߂�
		eData.Start(allTimeOnce);
		startIndex++;
		t = 0.0f;
	}


	//�⊮�p���W
	Vector3 p0, p1, p2, p3;
	//�n�_�ƏI�_�ɂ͒ǉ��Ń_�~�[��p�ӂ��Ă���̂ŕ⊮����_�̐���(����_-2)
	size_t lerpMax = controllPoints.size() - 2;

	//�ŏ��̕�ԁA�Ō�̕�Ԃł͂��ꂼ��p0,p3�Ƀ_�~�[���g��
	if (startIndex == 0) {
		//n = 0��p0(�_�~�[) p0 p1 p2
		p0 = dummyPoints[0];
	}
	else if (startIndex  < 0) {
		currentPos = dummyPoints[0];
		return;
	}
	else {
		p0 = controllPoints[startIndex - 1];
	}

	if (startIndex == lerpMax) {
		p3 = dummyPoints[1];
	}
	else if (startIndex > lerpMax) {
		currentPos = dummyPoints[1];
		return;
	}
	else {

		p3 = controllPoints[startIndex + 2];
	}

	p1 = controllPoints[startIndex];
	p2 = controllPoints[startIndex+1];

	Vector3 position = 0.5 * (((2 * p1) + (-p0 + p2) * t) +
		(((2 * p0) - (5 * p1) + (4 * p2) - p3) * (t * t)) +
		((-p0 + (3 * p1) - (3 * p2) + p3) * (t * t * t)));

	currentPos = position;
}
