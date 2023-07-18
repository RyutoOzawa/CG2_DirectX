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


	Vector3 result{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0]) ,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1]) ,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2])
	};

	return result;
}
Vector3 Matrix4::transformDivW(const Vector3& v, const Matrix4& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];


	Vector3 result{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) ,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) ,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2])
	};

	result /= w;

	return result;
}
Matrix4 Matrix4::CreateMatRot(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	Vector3 rX, rY, rZ;
	rZ = target - pos;
	rZ.normalize();

	rX = up.cross(rZ);
	rX.normalize();

	rY = rZ.cross(rX);
	rX.normalize();

	Matrix4 result = {
		rX.x,rX.y,rX.z,0,
		rY.x,rY.y,rY.z,0,
		rZ.x,rZ.y,rZ.z,0,
		   0,   0,   0,1
	};

	return result;
}
//
//Matrix4& operator*=(Matrix4& m1, const  Matrix4& m2)
//{
//	Matrix4 result{ 0 };
//
//	for (int i = 0; i < 4; i++) {
//		for (int j = 0; j < 4; j++) {
//			for (int k = 0; k < 4; k++) {
//				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
//			}
//		}
//	}
//
//	m1 = result;
//	return m1;
//}

Matrix4 Matrix4::Inverse()
{
	Matrix4 mat, result, identity;
	mat = *this;

	identity.identity();
	float sweep[4][8];

	//掃き出し法で使う行列の左側には元の行列、右には単位行列を代入
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			sweep[i][j] = mat.m[i][j];
			sweep[i][(j + 4)] = identity.m[i][j];
		}
	}

	//逆行列計算開始
	for (int k = 0; k < 4; k++) {

		//ゼロ除算対策
		double max = fabs(sweep[k][k]);
		int maxI = k;
		for (int i = k + 1; i < 4; i++) {
			if (fabs(sweep[i][k]) > max) {
				max = fabs(sweep[i][k]);
				maxI = i;
			}
		}

		//絶対値の最大値が0なら逆行列無し。単位行列を返す
		if (fabs(sweep[maxI][k]) <= 0.000001f) {
			return identity;
		}

		//該当行との入れ替え
		if (k != maxI) {
			for (int i = 0; i < 8; i++) {
				float temp = sweep[maxI][i];
				sweep[maxI][i] = sweep[k][i];
				sweep[k][i] = temp;
			}
		}


		//[k][k]に掛けたら1になる値を求め、k行に掛け算
		float x = 1 / sweep[k][k];
		for (int i = 0; i < 8; i++) {
			sweep[k][i] *= x;
		}

		//この時点でsweep[k][k]は1になっている
		//次に、[i][k]が0になるようにk行を -i倍
		for (int i = 0; i < 4; i++) {
			//i = kなら飛ばす
			if (i == k) {
				continue;
			}

			float num = -sweep[i][k];

			for (int j = 0; j < 8; j++) {
				sweep[i][j] += sweep[k][j] * num;
			}

		}
	}

	//掃き出し終了。sweepの右四列をリザルトに入れる
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = sweep[i][j + 4];
		}
	}

	*this = result;

	return *this;
}

Matrix4 Matrix4::CreateViewMat(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	Matrix4 result;
	//カメラのワールド行列を作成
	Vector3 cameraVecZ = target - eye;
	cameraVecZ.normalize();
	Vector3 cameraVecX = up.cross(cameraVecZ);
	cameraVecX.normalize();
	Vector3 cameraVecY = cameraVecZ.cross(cameraVecX);
	cameraVecY.normalize();

	result = {
		cameraVecX.x,cameraVecX.y,cameraVecX.z,0,
		cameraVecY.x,cameraVecY.y,cameraVecY.z,0,
		cameraVecZ.x,cameraVecZ.y,cameraVecZ.z,0,
				   0,           0,           0,1
	};


	//作った行列を逆行列に
	result.Inverse();

	//カメラの平行移動を計算
	Vector3 translation, eyeNegate = eye * -1;
	translation.x = cameraVecX.dot(eyeNegate);
	translation.y = cameraVecY.dot(eyeNegate);
	translation.z = cameraVecZ.dot(eyeNegate);

	result.m[3][0] = translation.x;
	result.m[3][1] = translation.y;
	result.m[3][2] = translation.z;
	result.m[3][3] = 1.0f;


	return result;
}

Matrix4 Matrix4::CreatePerspectiveProjection(float fovY, float aspectRatio, float nearZ, float farZ)
{
	Matrix4 result;

	float h = 1 / tanf(fovY / 2.0f);
	float w = h / aspectRatio;
	float z = farZ / (farZ - nearZ);
	float cameraZ = (-nearZ * farZ) / (farZ - nearZ);

	result = {
		w,0,0,0,
		0,h,0,0,
		0,0,z,1,
		0,0,cameraZ,0
	};

	return result;
}

Matrix4 Matrix4::CreateParallelProjection(float windowW, float windowH)
{
	Matrix4 result;


	float m00, m11;
	m00 = 2 / windowW;
	m11 = -2 / windowH;


	result =
	{
		 m00,0.0f,0.0f,0.0f,
		0.0f, m11,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
	   -1.0f,1.0f,0.0f,1.0f,
	};

	return result;
}

Matrix4& Matrix4::operator*=(const Matrix4& mat)
{
	Matrix4 result{};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m[i][k] * mat.m[k][j];
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result.m[i][j];
		}
	}

	*this = result;
	return *this;

}

bool Matrix4::operator==(const Matrix4& mat)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (m[i][j] != mat.m[i][j]) {
				return false;
			}
		}
	}

	return true;
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
