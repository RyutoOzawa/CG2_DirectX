#include "Matrix4.h"
#include "Vector3.h"
#include <cmath>

Matrix4 Matrix4::identity()
{
	static const Matrix4 result{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	*this = result;
	return *this;
}

Matrix4 Matrix4::scale(const Vector3& s)
{
	Matrix4 result{
		s.x,0.0f,0.0f,0.0f,
		0.0f,s.y,0.0f,0.0f,
		0.0f,0.0f,s.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

Matrix4 Matrix4::rotateX(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cos,sin,0.0f,
		0.0f,-sin,cos,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

Matrix4 Matrix4::rotateY(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result{
	   cos,0.0f,-sin,0.0f,
	   0.0f,1.0f,0.0f,0.0f,
	   sin,0.0f,cos,0.0f,
	   0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

Matrix4 Matrix4::rotateZ(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result{
			cos,sin,0.0f,0.0f,
			-sin,cos,0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f,
			0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

Matrix4 Matrix4::translate(const Vector3& t)
{
	Matrix4 result{
	1.0f,0.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,0.0f,
	0.0f,0.0f,1.0f,0.0f,
	t.x,t.y,t.z,1.0f
	};

	return result;
}

Vector3 Matrix4::transform(const Vector3& v, const Matrix4& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];


	Vector3 result{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
	};

	return result;
}

Matrix4& operator*=(Matrix4& m1, const  Matrix4& m2)
{
	Matrix4 result{ 0 };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	m1 = result;
	return m1;
}

Matrix4 Matrix4::Inverse()
{
	Matrix4 mat, result, identity;
	mat = *this;

	
	
	Matrix4 temp{
	2.3f,4.7f,8.0f,4.0f,
	7.3f,8.3f,4.7f,9.0f,
	4.2f,4.2f,1.0f,4.0f,
	7.6f,1.1f,4.2f,0.0f
	};

	mat = temp;

	identity.identity();
	float sweep[4][8];

	//�|���o���@�Ŏg���s��̍����ɂ͌��̍s��A�E�ɂ͒P�ʍs�����
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			sweep[i][j] = mat.m[i][j];
			sweep[i][(j + 4)] = identity.m[i][j];
		}
	}

	//�s��[0][0]�ɒ��ڂ���
	//mat.m[0][0]��1�łȂ��Ȃ�m[0][0]��m[0]������
	if (sweep[0][0] != 1) {
		for (int i = 0; i < 8; i++) {
			sweep[0][i] /= sweep[0][0];
		}
	}
	//���̎��_��mat.m[0][0]��1�ɂȂ��Ă���
	//���ɁAmat.m[1][0],mat.m[2][0]��0�ɂȂ�悤��0�s�ڂ�-10,-20�Ŋ|���Ĉ����Z
	for (int i = 0; i < 3; i++) {
		if (sweep[i + 1][0] != 0) {
			for (int j = 0; j < 8; j++) {
				sweep[i + 1][j] += sweep[0][j] * -sweep[i + 1][j];
			}
		}
	}

	//�s��[1][1]�ɒ���
	//[1][1]��1�ɂȂ��Ă��Ȃ���Γ��l�ōs������
	if (sweep[1][1] != 1) {
		for (int i = 0; i < 8; i++) {
			sweep[1][i] /= sweep[1][1];
		}
	}
	//���̎��_��[1][1]��1�ɂȂ��Ă���

	
	//*this = result;

	return *this;
}

const  Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result = m1;

	return result *= m2;
}

const Vector3 operator*(const Vector3& v, const Matrix4& m)
{
	Matrix4 result;

	return result.transform(v, m);
}
