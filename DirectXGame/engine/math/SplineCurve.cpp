#include "SplineCurve.h"
#include"ImguiManager.h"
#include"DebugLine.h"

void SplineCurve::Start(float allTime, bool isLoop)
{
	this->allTime = allTime;

	//��Ԃ��s����
	int32_t lerpCount;

	//��Ԃ̉񐔂͔z��̑���-2
	lerpCount = (int32_t)controllPoints.size() - 1;

	if (isLoop) {
		lerpCount++;
	}

	//��Ԃ̑S�̎��ԁ���Ԃ̉񐔂ň��ԓ�����̎��Ԃ��v�Z
	allTimeOnce = allTime / (float)lerpCount;

	//�_�~�[���W�̃Z�b�g
	dummyPoints[0] = controllPoints.front();
	dummyPoints[1] = controllPoints.back();

	currentPos = controllPoints[0];

	//���[�v����t���O
	this->isLoop = isLoop;

	//�J�n���̗v�f�ԍ������������ăC�[�W���O�J�n
	startIndex = 0;
	eData.Start(allTimeOnce);
	isActive = true;

	count = 0;

}

void SplineCurve::Update()
{


	if (!isActive) {
		return;
	}

	count++;
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


	//ImGui::Text("count %d", count);
	//ImGui::Text("start index %d", startIndex);
	//ImGui::Text("once time %f", allTimeOnce);
	//ImGui::Text("progress %f", progress);

	//�⊮�p���W
	Vector3 p0, p1, p2, p3;
	int32_t index0, index1, index2, index3;
	//�n�_�ƏI�_�ɂ͒ǉ��Ń_�~�[��p�ӂ��Ă���̂ŕ⊮����_�̐���(����_-2)
	size_t lerpMax = 0;

	//���[�v����Ȃ�S����_���n�_�ɂȂ�
	if (isLoop) {
		lerpMax = controllPoints.size() - 1;
	}
	else {
		lerpMax = controllPoints.size() - 2;
	}

	//���݂̐i�s�x�̍X�V
	progress = ((allTimeOnce * startIndex) + (t * allTimeOnce)) / allTime;

	//�ŏ��̕�ԁA�Ō�̕�Ԃł͂��ꂼ��p0,p3�Ƀ_�~�[���g��
	if (!isLoop) {
		if (startIndex == lerpMax) {
			p3 = dummyPoints[1];
		}
		else if (startIndex > lerpMax) {
			currentPos = dummyPoints[1];
			isActive = false;
			progress = 1.0f;
			return;
		}
		else {

			p3 = controllPoints[startIndex + 2];
		}

		if (startIndex == 0) {
			//n = 0��p0(�_�~�[) p0 p1 p2
			p0 = dummyPoints[0];
		}
		else if (startIndex < 0) {
			currentPos = dummyPoints[0];
			return;
		}
		else {
			p0 = controllPoints[startIndex - 1];
		}
		p1 = controllPoints[startIndex];
		p2 = controllPoints[startIndex + 1];

		Vector3 position = 0.5 * (((2 * p1) + (-p0 + p2) * t) +
			(((2 * p0) - (5 * p1) + (4 * p2) - p3) * (t * t)) +
			((-p0 + (3 * p1) - (3 * p2) + p3) * (t * t * t)));

		currentPos = position;
	}
	else {//���[�v����Ȃ�
		int32_t backIndex;
		backIndex = (int32_t)controllPoints.size() - 1;

		//��ԊJ�n�C���f�b�N�X���ő�l�𒴂��Ȃ��悤�ɂ���
		if (startIndex > lerpMax) {
			startIndex = 0;
		}

		if (startIndex == 0) {
			index0 = backIndex;
			index1 = startIndex;
			index2 = startIndex + 1;
			index3 = startIndex + 2;
		}
		else if (startIndex == backIndex - 1) {
			index0 = startIndex - 1;
			index1 = startIndex;
			index2 = startIndex + 1;
			index3 = 0;
		}
		else if (startIndex == backIndex) {
			index0 = startIndex - 1;
			index1 = startIndex;
			index2 = 0;
			index3 = 1;
		}
		else {
			index0 = startIndex - 1;
			index1 = startIndex;
			index2 = startIndex + 1;
			index3 = startIndex + 2;
		}

		p0 = controllPoints[index0];
		p1 = controllPoints[index1];
		p2 = controllPoints[index2];
		p3 = controllPoints[index3];

		/*	ImGui::Text("p0:%d", index0);
			ImGui::Text("p1:%d", index1);
			ImGui::Text("p2:%d", index2);
			ImGui::Text("p3:%d", index3);*/


		Vector3 position = 0.5 * (((2 * p1) + (-p0 + p2) * t) +
			(((2 * p0) - (5 * p1) + (4 * p2) - p3) * (t * t)) +
			((-p0 + (3 * p1) - (3 * p2) + p3) * (t * t * t)));

		currentPos = position;
	}

}

void SplineCurve::DrawCurve(const Vector4& color, const Vector3& offset)
{
	std::vector<Vector3> drawPoints;

	//�e��Ԃ�100�����o��
	const size_t lineCount = 100;
	std::vector<Vector3> point = controllPoints;
	if (isLoop) {
		point.push_back(point.front());
	}

	for (size_t j = 0; j < point.size() - 1; j++) {
		Vector3 p0, p1, p2, p3;
		if (j == 0) {
			p0 = point[j];
		}
		else {
			p0 = point[j - 1];
		}

		if (j == point.size() - 2) {
			p3 = point.back();
		}
		else {
			p3 = point[j + 1];
		}

		p1 = point[j];
		p2 = point[j + 1];



		for (size_t i = 0; i < lineCount; i++) {
			float t = 1.0f / lineCount * i;
			Vector3 pos = 0.5 * (((2 * p1) + (-p0 + p2) * t) +
				(((2 * p0) - (5 * p1) + (4 * p2) - p3) * (t * t)) +
				((-p0 + (3 * p1) - (3 * p2) + p3) * (t * t * t)));
			pos += offset;
			drawPoints.push_back(pos);
		}
	}

	DebugLine::Draw(drawPoints, color);

}
