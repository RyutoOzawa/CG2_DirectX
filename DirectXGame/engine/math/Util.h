//���[�e�B���e�B�N���X
//�F�X�֗��Ȋ֐��B���܂Ƃ߂��N���X

#define PI 3.1415926f
#include"Vector2.h"
#include"Vector3.h"
#include"Matrix4.h"

/// <summary>
/// �����_��
/// </summary>
/// <param name="num1">�l1</param>
/// <param name="num2">�l2</param>
/// <returns>1��2�̊Ԃ��烉���_���Ȓl</returns>
float Random(float num1, float num2);

//�J���[�R�[�h��(R,G,B)�ɕϊ�
int RGBColorCode(const Vector3& rgb);

//(R,G,B)���J���[�R�[�h�ɕϊ�
Vector3 ColorCodeRGB(int colorCode);

//�X�N���[�����W���烏�[���h���W�ɕϊ�
Vector3 ConvertScreenToWorld(const Vector2& v,float distanceZ, const Matrix4& matViewProjectionViewPort);

//�C�[�W���O�A�E�g
float EaseOut(float t);

//���`���
float Lerp(float before, float after, float t);

//�N�����v
float Clump(float num, float min, float max);