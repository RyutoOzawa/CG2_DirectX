cbuffer WorldTransform : register(b0)
{
	matrix world; // �R�c�ϊ��s��
};

cbuffer ViewProjection : register(b1)
{
	matrix view;
	matrix projection;
	float3 cameraPos;
}

cbuffer Material : register(b2)
{
	float3 m_ambient : packoffset(c0);//�A���r�G���g�W��
	float3 m_diffuse : packoffset(c1);//�f�B�t���[�Y�W��
	float3 m_specular : packoffset(c2);//�X�y�L�����[�W��
	float m_alpha : packoffset(c2.w);//�A���t�@
}



// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float4 worldpos : POSITION;	//���[���h���W
	float3 normal :NORMAL; // �@���x�N�g��
	float2 uv  :TEXCOORD; // uv�l
};
