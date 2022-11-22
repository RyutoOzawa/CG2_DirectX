#include "Matrix4.h"
#include <cmath>

Matrix4::Matrix4()
{
	float result[4][4]
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	// ���
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result[i][j];
		}
	}
}

Matrix4::Matrix4(float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	m[0][0] = m00, m[0][1] = m01, m[0][2] = m02, m[0][3] = m03,
	m[1][0] = m10, m[1][1] = m11, m[1][2] = m12, m[1][3] = m13,
	m[2][0] = m20, m[2][1] = m21, m[2][2] = m22, m[2][3] = m23,
	m[3][0] = m30, m[3][1] = m31, m[3][2] = m32, m[3][3] = m33;

}

//�P�ʍs������߂�
Matrix4 Matrix4::identity()
{
	const Matrix4 result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};

	return *this;
}

Matrix4 Matrix4::MakeInverse(const Matrix4* mat)
{
	//�|���o���@���s���s��
	float sweep[4][8]{};
	//�萔�{�p
	float constTimes = 0.0f;
	//���e����덷
	float MAX_ERR = 1e-10f;
	//�߂�l�p
	Matrix4 retMat;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//weep�̍����ɋt�s������߂�s����Z�b�g
			sweep[i][j] = mat->m[i][j];

			//sweep�̉E���ɒP�ʍs����Z�b�g
			sweep[i][4 + j] = identity().m[i][j];
		}
	}

	//�S�Ă̗�̑Ίp�����ɑ΂���J��Ԃ�
	for (int i = 0; i < 4; i++)
	{
		//�ő�̐�Βl�𒍖ڑΊp�����̐�Βl�Ɖ���
		float max = std::fabs(sweep[i][i]);
		int maxIndex = i;

		//i��ڂ��ő�̐�Βl�ƂȂ�s��T��
		for (int j = i + 1; j < 4; j++)
		{
			if (std::fabs(sweep[j][i]) > max)
			{
				max = std::fabs(sweep[j][i]);
				maxIndex = j;
			}
		}

		if (fabs(sweep[maxIndex][i]) <= MAX_ERR)
		{
			//�t�s��͋��߂��Ȃ�
			return identity();
		}

		//����(1):i�s�ڂ�maxIndex�s�ڂ����ւ���
		if (i != maxIndex)
		{
			for (int j = 0; j < 8; j++)
			{
				float tmp = sweep[maxIndex][j];
				sweep[maxIndex][j] = sweep[i][j];
				sweep[i][j] = tmp;
			}
		}

		//sweep[i][i]�Ɋ|�����1�ɂȂ�l�����߂�
		constTimes = 1 / sweep[i][i];

		//����(2):p�s�ڂ�a�{����
		for (int j = 0; j < 8; j++)
		{
			//����ɂ��sweep[i][i]��1�ɂȂ�
			sweep[i][j] *= constTimes;
		}

		//����(3)�ɂ��i�s�ڈȊO�̍s��i��ڂ�0�ɂ���
		for (int j = 0; j < 4; j++)
		{
			if (j == i)
			{
				//i�s�ڂ͂��̂܂�
				continue;
			}

			//i�s�ڂɊ|����l�����߂�
			constTimes = -sweep[j][i];

			for (int k = 0; k < 8; k++)
			{
				//j�s�ڂ�i�s�ڂ�a�{�����s�𑫂�
				//����ɂ��sweep[j][i]��0�ɂȂ�
				sweep[j][k] += sweep[i][k] * constTimes;
			}
		}
	}

	//sweep�̉E������mat�̋t�s��
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			retMat.m[i][j] = sweep[i][4 + j];
		}
	}

	return retMat;
}

//�g��k���s������߂�
Matrix4 Matrix4::scale(const Vector3& s)
{
	float result[4][4]
	{
		s.x ,0.0f,0.0f,0.0f,
		0.0f,s.y ,0.0f,0.0f,
		0.0f,0.0f,s.z ,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	// ���
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result[i][j];
		}
	}

	return *this;
}

// x ������̉�]�s������߂�
Matrix4 Matrix4::rotateX(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	float result[4][4]
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f, cos, sin,0.0f,
		0.0f,-sin, cos,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	// ���
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result[i][j];
		}
	}

	return *this;
}

// y ������̉�]�s������߂�
Matrix4 Matrix4::rotateY(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	float result[4][4]
	{
		cos ,0.0f,-sin,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		sin ,0.0f, cos,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	// ���
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result[i][j];
		}
	}

	return *this;
}

// z ������̉�]�s������߂�
Matrix4 Matrix4::rotateZ(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	float result[4][4]
	{
		cos , sin,0.0f,0.0f,
		-sin, cos,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	// ���
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result[i][j];
		}
	}

	return *this;
}

// ���s�ړ��s������߂�
Matrix4 Matrix4::translate(const Vector3& t)
{
	float result[4][4]
	{
		1.0f,1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		t.x ,t.y ,t.z ,1.0f
	};

	// ���
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result[i][j];
		}
	}

	return *this;
}

// ���W�ϊ��i�x�N�g���ƍs��̊|���Z�j���s��transform �֐����쐬����@(�����ϊ��ɂ��Ή����Ă���)
Vector3 Matrix4::transform(const Vector3& v, const Matrix4& m)
{
	float W = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
	Vector3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / W,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / W,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / W
	};

	return result;
}

// ������Z�q�@*=�@�I�[�o�[���[�h�֐��i�s��ƍs��̐ρj
Matrix4& Matrix4::operator*=(const Matrix4& m1)
{
	Matrix4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float Total = 0.0f;
			for (int k = 0; k < 4; k++) {
				Total += m[i][k] * m1.m[k][j];
			}
			result.m[i][j] = Total;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result.m[i][j];
		}
	}

	return *this;
}

// �Q�����Z�q�@*�@�̃I�[�o�[���[�h�֐��i�s��ƍs��̐ρj
Matrix4 Matrix4::operator*(const Matrix4& m1)
{
	return *this *= m1;
}

// �Q�����Z�q�@*�@�̃I�[�o�[���[�h�֐��i�x�N�g���ƍs��̐ρj
Vector3 operator*(const Vector3& v, const Matrix4& m)
{
	float W = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
	Vector3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / W,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / W,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / W
	};
	return result;
}