#pragma once

#include"Vector3.h"
class Matrix4
{
public:

	float m[4][4];


	//単位行列を求める
	Matrix4 identity();

	//拡大縮小行列の設定
	Matrix4 scale(const Vector3& s);

	//回転行列の設定
	Matrix4 rotateX(float angle);
	Matrix4 rotateY(float angle);
	Matrix4 rotateZ(float angle);

	//平行移動行列作成
	Matrix4 translate(const Vector3& t);

	//座標変換（ベクトルと行列の掛け算）
	Vector3 transform(const Vector3& v, const Matrix4& m);

	//逆行列
	Matrix4 Inverse();

	Matrix4 CreateViewMat(const Vector3& eye, const Vector3& target, const Vector3& up );

	Matrix4 CreatePerspectiveProjection(float fovY,float aspectRatio,float nearZ,float farZ);

	Matrix4 CreateParallelProjection(float windowW,float windowH);

	//演算子オーバーロード
	Matrix4& operator*=(const Matrix4& mat);

};

//2項演算子オーバーロード
const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
const Vector3 operator*(const Vector3& v, const Matrix4& m);