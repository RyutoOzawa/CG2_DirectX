#pragma once
//�X�v���C���Ȑ��N���X
//�X�v���C����Ԃ��s��

#include"EasingData.h"
#include"Vector3.h"
#include<vector>
#include"Vector4.h"

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
	/// <param name="isLoop">���[�v���邩</param>
	void Start(float allTime_,bool isLoop_ = false);

	//�X�V
	void Update();

	//getter
	Vector3 GetPosition()const { return currentPos; }
	float GetProgress()const { return progress; }

	//�Ȑ��`��(�f�o�b�O�p)
	void DrawCurve(const Vector4& color,const Vector3& offset = {0,0,0});
};

