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

void LerpParticle::Add(int t, const Vector3& start, const Vector3& end, float scaleStart, float scaleEnd, InterType type)
{
	this->start = start;
	this->end = end;
	this->scale = scaleStart;
	this->scaleStart = scaleStart;
	this->scaleEnd = scaleEnd;
	this->num_frame = t;
	this->interType = type;
}
