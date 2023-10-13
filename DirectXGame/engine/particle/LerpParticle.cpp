//���`��ԃp�[�e�B�N���N���X�@
//��{�̃p�[�e�B�N���N���X���p�����A�p�[�e�B�N���̈ړ�����`��Ԃōs���N���X

#include "LerpParticle.h"
#include"Util.h"

void LerpParticle::Update()
{
	//�o�߃t���[���J�E���g
	frame++;
	//���ݎ��Ԍv�Z
	float f = (float)frame / num_frame;

	//�X�P�[���̕��
	scale = (scaleEnd - scaleStart) * f;
	scale += scaleStart;

	//���W���
	switch (interType)
	{
	case InterType::Lerp:
		break;
	case InterType::EaseIn:
		//TODO:�C�[�W���O�ł���悤�ɂ���

		break;
	case InterType::EaseOut:
		f = EaseOut(f);

		break;
	default:
		break;
	}

	position = Vector3::Lerp(start, end, f);

	

}

void LerpParticle::Add(int t, const Vector3& start_, const Vector3& end_, float scaleStart_, float scaleEnd_, InterType type)
{
	start = start_;
	end = end_;
	scale = scaleStart_;
	scaleStart = scaleStart_;
	scaleEnd = scaleEnd_;
	num_frame = t;
	interType = type;
}
