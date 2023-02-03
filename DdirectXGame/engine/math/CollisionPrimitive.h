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



