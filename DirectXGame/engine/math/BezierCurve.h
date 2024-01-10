#pragma once
//�x�W�G�Ȑ��N���X
//n���x�W�G��Ԃ��s��
//����_��������ł��ݒ�ł��邽�߁An���ɑΉ��\

#include"EasingData.h"
#include"Vector3.h"
#include"Vector4.h"
#include<vector>

class BezierCurve
{
private:
	//����_�z��
	std::vector<Vector3> controllPoints;
	//���ݍ��W
	Vector3 currentPos{};
	//�⊮�p�f�[�^
	Easing::EasingData eData;
	//���[�v���邩
	bool isLoop = false;
	//�i�s�x
	bool isActive = false;
	float allTime = 0.0f;

public:
	//����_�̃Z�b�g
	void SetPositions(std::vector<Vector3> points) { this->controllPoints = points; }

	/// <summary>
	/// ��Ԃ̊J�n
	/// </summary>
	/// <param name="allTime">��Ԃ̎���</param>
	/// <param name="isloop">���[�v���邩</param>
	void Start(float allTime_, bool isLoop_);

	//�X�V
	void Update();

	//getter
	Vector3 GetPosition()const { return currentPos; }


	/// <summary>
	/// �Ȑ��`��
	/// </summary>
	/// <param name="color">�F</param>
	/// <param name="offset">�I�t�Z�b�g(�f�t�H���g��{0,0,0})</param>
	void DrawCurve(const Vector4& color, const Vector3& offset = { 0,0,0 });

	//����_�����炵�Ă����ċA�֐�
	Vector3 BezierPoints(std::vector<Vector3> posints,float t);
};

