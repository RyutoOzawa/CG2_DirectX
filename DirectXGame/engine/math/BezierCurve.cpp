//�x�W�G�Ȑ��N���X
//n���x�W�G��Ԃ��s��
//����_��������ł��ݒ�ł��邽�߁An���ɑΉ��\

#include "BezierCurve.h"
#include"DebugLine.h"

void BezierCurve::Start(float allTime_, bool isLoop_)
{
	allTime = allTime_;
	eData.Start(allTime);
	currentPos = controllPoints.front();
	isLoop = isLoop_;
}

void BezierCurve::Update()
{
	eData.Update();

	if (eData.GetTimeRate() < 1.0f) {
		//���[�v����Ȃ�z��̖����ɐ擪����������Ŋ֐���
		std::vector<Vector3> points = controllPoints;
		if (isLoop) {
			points.push_back(points.front());
		}

		currentPos = BezierPoints(points,eData.GetTimeRate());
	}
	else {
		//���[�v����Ă���Ȃ������x�J�n
		if (isLoop) {
			Start(allTime, true);
		}
		else {
			currentPos = controllPoints.back();
		}
	}

}

void BezierCurve::DrawCurve(const Vector4& color, const Vector3& offset)
{
	std::vector<Vector3> points = controllPoints;
	if (isLoop) {
		points.push_back(points.front());
	}

	//����_1�ɂ��`��p�_��96��
	size_t pointCount = points.size() * 96;
	//�`��p���W�z��
	std::vector<Vector3> drawPoints;
	for (size_t i = 0; i < pointCount; i++) {
		//1.0��pointCount�ŕ������ē_������o���Ă���
		float t = 1.0f / pointCount * i;
		Vector3 p = BezierPoints(points, t);
		//�I�t�Z�b�g���Z
		p += offset;
		drawPoints.push_back(p);

	}

	DebugLine::Draw(drawPoints, color);

}

Vector3 BezierCurve::BezierPoints(std::vector<Vector3> points,float t)
{

	//�v�f��2�Ȃ炻��2�_�̕�ԂłƂ�
	size_t arraySize = points.size();
	if (arraySize == 2) {
		return Vector3::Lerp(points.front(), points.back(), t);
	}
	else if (arraySize == 1) {
		return points.front();
	}
	else if (arraySize == 0) {
		return Vector3(0,0,0);
	}
	else {

		//�����̔z�񂩂�V�����z������
		std::vector<Vector3> newPoints;
		for (size_t i = 0; i < points.size() - 1; i++) {
			Vector3 point;
			point = Vector3::Lerp(points[i], points[i + 1], t);
			newPoints.push_back(point);
		}

		//�v�f����2�ɂȂ�܂ōċA�Ăяo��
		return BezierPoints(newPoints, t);

	}
}
