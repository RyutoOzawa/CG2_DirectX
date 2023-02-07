#include "Collision.h"
#include<cmath>

bool Collision::ColSphereToPlane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
	//座標系の頂点から球の中心座標への距離
	float distV = sphere.pos.dot(plane.normal);
	//平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = distV - plane.distance;
	//距離の絶対値が半径より大きければあたっていない
	if (fabsf(dist) > sphere.radius)return false;

	//疑似交点を計算
	if (inter) {
		//平面上の最近接点を、疑似交点とする
		*inter = -dist * plane.normal + sphere.pos;
	}

	return true;
}

void Collision::ClosestPtPointToTriangle(const Vector3& point, const Triangle& triangle, Vector3* closest)
{
	//pointがp0の外側の頂点領域の中にあるかどうかチェック
	Vector3 p0_p1 = triangle.p1 - triangle.p0;
	Vector3 p0_p2 = triangle.p2 - triangle.p0;
	Vector3 p0_pt = point - triangle.p0;

	float d1 = p0_p1.dot(p0_pt);
	float d2 = p0_p2.dot(p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f) {
		//p0が最近傍
		*closest = triangle.p0;
		return;
	}

	//pointがp1の外側の頂点領域にあるかどうかチェック
	Vector3 p1_pt = point - triangle.p1;

	float d3 = p0_p1.dot(p1_pt);
	float d4 = p0_p2.dot(p1_pt);

	if (d3 >= 0.0f && d4 <= d3) {
		//p1が最近傍
		*closest = triangle.p1;
		return;
	}

	//pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
		float v = d1 / (d1 - d3);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	//pointがp2の外側の頂点領域の中にいるかどうかチェック
	Vector3 p2_pt = point - triangle.p2;

	float d5 = p0_p1.dot(p2_pt);
	float d6 = p0_p2.dot(p2_pt);

	if (d6 >= 0.0f && d5 <= d6) {
		*closest = triangle.p2;
		return;
	}

	//pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
		float w = d2 / (d2 - d6);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	//pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;

}

bool Collision::ColSphereToTriangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter)
{
	Vector3 p;
	//球の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPointToTriangle(sphere.pos, triangle, &p);
	//点pと球の中心の差分ベクトル
	Vector3 v = p - sphere.pos;
	//距離を求める
	//同じベクトル同士の内積は三平方の定理のルート内部の式と一致する
	float len = v.dot(v);

	//球と三角形の距離が半径以下ならあたっていない
	if (len > sphere.radius * sphere.radius)return false;
	//疑似交点を計算
	if (inter) {
		//三角形上の最近接点pを疑似交点とする
		*inter = p;
	}
	return true;
}

bool Collision::ColRayToPlane(const Ray& ray, const Plane& plane, float* distance, Vector3* inter)
{
	const float epsilon = 1.0e-5f;//誤差吸収用の微小な値
	//面法線とレイの方向ベクトルの内積
	float d1 = plane.normal.dot(ray.dir);
	//裏面には当たらない
	if (d1 > -epsilon) { return false; }
	//始点と原点の距離(平面の法線方向)
	//面法線とレイの始点座標(位置ベクトル)の内積
	float d2 = plane.normal.dot(ray.start);
	//始点と平面の距離(平面の法線方向)
	float dist = d2 - plane.distance;
	//始点と平面の距離(レイ方向)
	float t = dist / -d1;
	//交点が始点より後ろにあるので、当たらない
	if (t < 0)return false;
	//距離を書き込む
	if (distance) { *distance = t; }
	//交点を計算
	if (inter) { *inter = ray.start + t * ray.dir; }
	return true;
}

bool Collision::ColRayToTriangle(const Ray& ray, const Triangle& triangle, float* distance, Vector3* inter)
{
	//三角形が乗っている平面を算出
	Plane plane;
	Vector3 interPlane;
	plane.normal = triangle.normal;
	plane.distance = triangle.normal.dot(triangle.p0);
	//レイと平面があたっていなければ、当たっていない
	if (!ColRayToPlane(ray, plane, distance, &interPlane)) { return false; }
	//レイと平面が当たっているので、距離と交点が書き込まれた
	//レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f;//誤差吸収用の微小な値
	Vector3 m;
	//辺p0_p1について
	Vector3 pt_p0 = triangle.p0 - interPlane;
	Vector3 p0_p1 = triangle.p1 - triangle.p0;
	m = pt_p0.cross(p0_p1);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (m.dot(triangle.normal) < -epsilon) { return false; }

	//辺p1_p2について
	Vector3 pt_p1 = triangle.p1 - interPlane;
	Vector3 p1_p2 = triangle.p2 - triangle.p1;
	m = pt_p1.cross(p1_p2);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (m.dot(triangle.normal) < -epsilon) { return false; }

	//辺p2_p0について
	Vector3 pt_p2 = triangle.p2 - interPlane;
	Vector3 p2_p0= triangle.p0 - triangle.p2;
	m = pt_p2.cross(p2_p0);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (m.dot(triangle.normal) < -epsilon) { return false; }

	//内側なので、当たっている
	if (inter) {
		*inter = interPlane;
	}

	return true;
}
