#pragma once

#include"CollisionPrimitive.h"

/// <summary>
/// 当たり判定ヘルパークラス
/// </summary>
class Collision
{
public:

	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">交点(平面上での接点)</param>
	/// <returns>交差してるかどうか</returns>
	static bool ColSphereToPlane(const Sphere& sphere, const Plane& plane,
		Vector3* inter = nullptr);

};

