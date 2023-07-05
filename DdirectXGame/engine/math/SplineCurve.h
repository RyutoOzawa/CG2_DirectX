#pragma once
#include"EasingData.h"
#include"Vector3.h"
#include<vector>

class SplineCurve
{
private:
	//����_�z��
	std::vector<Vector3> controllPoints;
	//���݂ǂ̐���_�����Ԃ��s���Ă��邩
	int32_t startIndex = 0;
	//�⊮�p�f�[�^
	EasingData eData;

public:
	//����_�̃Z�b�g
	void SetPositions(std::vector<Vector3> points) { this->controllPoints = points; }

	/// <summary>
	/// ��Ԃ̊J�n
	/// </summary>
	/// <param name="allTime">��Ԃɂ����鎞��</param>
	void Start(float allTime);

	//�X�V
	void Update();

};

