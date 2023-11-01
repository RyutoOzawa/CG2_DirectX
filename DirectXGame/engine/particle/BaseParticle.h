#pragma once
//��{�p�[�e�B�N���N���X
//�}�l�[�W���ɂ���Đ��������p�[�e�B�N���̃p�����[�^��������

#include"Vector3.h"
#include"Vector4.h"

class BaseParticle
{

protected:

	//���W
	Vector3 position;
	//���݃X�P�[��
	float scale = 1.0f;
	//���݃t���[��
	int frame = 0;
	//�I���t���[��
	int num_frame = 0;
	//�F
	Vector4 color = { 1,1,1,1 };

public:
	//�X�V
	virtual void Update();

	/// <summary>
	/// �p�[�e�B�N���ǉ�
	/// </summary>
	/// <param name="life">��������</param>
	/// <param name="position_">���W</param>
	/// <param name="velocity_">���x</param>
	/// <param name="accel_">�����x</param>
	/// <param name="scaleStart_">�J�n�X�P�[��</param>
	/// <param name="scaleEnd_">�I���X�P�[��</param>
	virtual void Add(int life, const Vector3& position_, const Vector3& velocity_,
		const Vector3& accel_, float scaleStart_, float scaleEnd_, Vector4 color_ ={1,1,1,1});

	//�����t���O
	bool IsAlive() { return frame > num_frame; }

	//getter
	Vector3 GetPosition()const { return position; }
	float GetScale()const { return scale; }

private:
	//�X�P�[���J�n�l
	float scaleStart = 1.0f;
	//�X�P�[���I���l
	float scaleEnd = 0.0f;
	//���x
	Vector3 velocity = {};
	//�����x
	Vector3 accel = {};
};

