#pragma once
#include"EasingData.h"
#include"Vector3.h"
#include<vector>

class SplineCurve
{
private:
	//����_�z��
	std::vector<Vector3> controllPoints;
	//����_�̊Ԃœ���o���邽�߂̎n�_�ƏI�_�̃R�s�[���W
	Vector3 dummyPoints[2]{};
	//���ݍ��W
	Vector3 currentPos{};
	//���݂ǂ̐���_�����Ԃ��s���Ă��邩
	int32_t startIndex = 0;
	//�⊮�p�f�[�^
	EasingData eData;
	//���ԓ�����̕�Ԏ���
	float allTimeOnce = 0.0f;
	//�n�_�ƏI�_���Ȃ��Ń��[�v���邩�ǂ���
	bool isLoop = false;
	//�S�̂̎���
	float allTime = 0.0f;
	//���݂̐i�s�x
	float progress = 0.0f;
	bool isActive = false;
	int count = 0;


public:
	//����_�̃Z�b�g
	void SetPositions(std::vector<Vector3> points) { this->controllPoints = points; }

	/// <summary>
	/// ��Ԃ̊J�n
	/// </summary>
	/// <param name="allTime">��Ԃɂ����鎞��</param>
	void Start(float allTime,bool isLoop = false);

	//�X�V
	void Update();

	Vector3 GetPosition()const { return currentPos; }

};

