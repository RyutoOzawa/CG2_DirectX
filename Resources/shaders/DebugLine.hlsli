cbuffer cbuff0 : register(b0)
{
	matrix world;//3D�s��
	float4 color;//�F
}

cbuffer cbuff1 : register(b1)
{
	matrix view;//�r���[�s��ƃv���W�F�N�V�����s��
	matrix projection;//�r���[�s��ƃv���W�F�N�V�����s��
}

//���_�V�F�[�_����s�N�Z���V�F�[�_�ւ̂����Ɏg���\����
struct VSOutput
{
	float4 svpos : SV_POSITION;//�V�X�e���p���_���W
};