#pragma once
#include"BaseParticle.h"

enum class InterType {
	Lerp,		//���`���
	EaseIn,		//�C�[�W���O(�x����)
	EaseOut,	//�C�[�W���O(�����x)
};

//�p�[�e�B�N����ՃN���X���p��
class LerpParticle : public BaseParticle
{
private:

	//�n�_
	Vector3 start = {};
	//�I�_
	Vector3 end = {};
	//�X�P�[���J�n�l
	float scaleStart = 1.0f;
	//�X�P�[���I���l
	float scaleEnd = 0.0f;

	//��Ԃ̎��
	InterType interType = InterType::Lerp;

public:
	void Update() override;

	void Add(int t, const Vector3& start_, const Vector3& end_, float scaleStart_, float scaleEnd_, InterType type);
};

