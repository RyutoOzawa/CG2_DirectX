cbuffer WorldTransform : register(b0)
{
	matrix mat; // �R�c�ϊ��s��
};

cbuffer Material : register(b1)
{
	float3 m_ambient : packoffset(c0);//�A���r�G���g�W��
	float3 m_diffuse : packoffset(c1);//�f�B�t���[�Y�W��
	float3 m_specular : packoffset(c2);//�X�y�L�����[�W��
	float m_alpha : packoffset(c2.w);//�A���t�@
}

cbuffer viewProjection : register(b2) 
{
	matrix view;
	matrix projection;
	float3 cameraPos;
}

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	//float4 worldpos : POSITION;	//���[���h���W
	float3 normal :NORMAL; // �@���x�N�g��
	float2 uv  :TEXCOORD; // uv�l
};
