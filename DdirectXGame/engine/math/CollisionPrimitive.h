#pragma once
#include"Vector3.h"

/// <summary>
/// 当たり判定プリミティブ
/// </summary>

/// <summary>
/// 球のデータ型
/// </summary>
struct Sphere {
	//座標
	Vector3 pos = {0,0,0};
	//半径
	float radius = 1.0f;
};

/// <summary>
/// 平面のデータ型
/// </summary>
struct Plane {
	//法線ベクトル
	Vector3 normal = { 0,1,0 };
	//原点(0,0)からの距離
	float distance = 0.0f;
};

/// <summary>
/// 法線付き三角形データ(時計回りが表面)
/// </summary>
class Triangle {
public:
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;
	//法線
	Vector3 normal;
};

/// <summary>
/// レイ(半直線)データ
/// </summary>
struct Ray {
	//始点座標
	Vector3 start{ 0,0,0 };
	//方向
	Vector3 dir{ 1,0,0 };
};