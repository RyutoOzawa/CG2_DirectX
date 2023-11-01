//��{�p�[�e�B�N���N���X
//�}�l�[�W���ɂ���Đ��������p�[�e�B�N���̃p�����[�^��������
#include "BaseParticle.h"

void BaseParticle::Update()
{
	//�o�߃t���[���J�E���g
	frame++;
	//����
	velocity += accel;
	//�ړ�
	position += velocity;
	//���ݎ��Ԍv�Z
	float f = (float)frame / num_frame;
	//�X�P�[���̕��
	scale = (scaleEnd - scaleStart) * f;
	scale += scaleStart;

}

void BaseParticle::Add(int life, const Vector3& position_, const Vector3& velocity_,
	const Vector3& accel_, float scaleStart_, float scaleEnd_,Vector4 color_)
{
	position = position_;
	velocity = velocity_;
	accel = accel_;
	num_frame = life;
	scale = scaleStart_;
	scaleStart = scaleStart_;
	scaleEnd = scaleEnd_;
	color = color_;
}
