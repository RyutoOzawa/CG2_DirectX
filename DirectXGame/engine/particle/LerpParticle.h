#pragma once
//���`��ԃp�[�e�B�N���N���X�@
//��{�̃p�[�e�B�N���N���X���p�����A�p�[�e�B�N���̈ړ�����`��Ԃōs���N���X

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
	//�X�V
	void Update() override;

	/// <summary>
	/// �p�[�e�B�N���ǉ�
	/// </summary>
	/// <param name="t">��������</param>
	/// <param name="start_">�n�_</param>
	/// <param name="end_">�I�_</param>
	/// <param name="scaleStart_">�J�n�X�P�[��</param>
	/// <param name="scaleEnd_">�I���X�P�[��</param>
	/// <param name="type">��Ԃ̎��</param>
	void Add(int t, const Vector3& start_, const Vector3& end_, float scaleStart_, float scaleEnd_, InterType type = InterType::Lerp, Vector4 color_ = {1,1,1,1});
};

