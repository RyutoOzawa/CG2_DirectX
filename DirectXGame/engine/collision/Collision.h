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

	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="triangle">三角形</param>
	/// <param name="closest">最近接点(出力用)</param>
	static void ClosestPtPointToTriangle(const Vector3& point, const Triangle& triangle, Vector3* closest);

	/// <summary>
	/// 球と法線付き三角形の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点(三角形上の最近接点)</param>
	/// <returns>交差しているかどうか</returns>
	static bool ColSphereToTriangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);

	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="distance">距離(出力用)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>交差しているか</returns>
	static bool ColRayToPlane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="distance">距離(出力用)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>交差しているかどうか</returns>
	static bool ColRayToTriangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="distance">距離(出力用)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>交差しているかどうか</returns>
	static bool ColRayToSphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// 球同士の当たり判定
	/// </summary>
	/// <param name="s1">球1</param>
	/// <param name="s2">球2</param>
	/// <param name="distance">距離(出力用)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>衝突しているか</returns>
	static bool ColSphereToSphere(const Sphere& s1, const Sphere& s2, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// 円と円の当たり判定(2D)
	/// </summary>
	/// <param name="c1">円1</param>
	/// <param name="c2">円2</param>
	/// <returns></returns>
	static bool ColCircleToCircle(const Circle& c1, const Circle& c2);
};

