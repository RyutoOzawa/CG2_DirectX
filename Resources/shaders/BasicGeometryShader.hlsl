#include"Obj.hlsli"



//�_�̓��͂���A�l�p�`���o��
[maxvertexcount(3)]
void main(
	triangle VSOutput input[3]:SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
	
	//4���_����
	for (uint i = 0; i < 3; i++) {
		GSOutput element;
		//���[���h���W�x�[�X�ł��炷
		element.svpos =input[i].svpos;
		element.normal =input[i].normal;
		element.uv =input[i].uv;
		output.Append(element);
	}
}