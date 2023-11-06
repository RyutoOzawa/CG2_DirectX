cbuffer cbuff0 : register(b0)
{
	matrix mat;//�r���{�[�h�ϊ��s��
	float4 color;//�F
}

cbuffer cbuff1 : register(b1)
{
	matrix view;//�J�����s��
	matrix projection;
}

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 pos : SV_POSITION; // ���_���W
	float scale : TEXCOORD;//�X�P�[��
};

//�W�I���g���V�F�[�_�[����s�N�Z���V�F�[�_�ւ̏o��
struct GSOutput {
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float2 uv  :TEXCOORD; // uv�l
};