#pragma once
//球体コライダークラス
//球体の当たり判定を持つクラス

#include"BaseCollider.h"
#include"CollisionPrimitive.h"

#include"Vector3.h"

class SphereCollider : public BaseCollider, public Sphere
{
public:
	SphereCollider(Vector3 offset_ = {0,0,0},float radius_ = 0):
	offset(offset_),radius(radius_) {
		//球形状をセット
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	inline void SetRadius(float radius_) { radius = radius_; }

private:
	//オブジェクト中心からのオフセット
	Vector3 offset;
	//半径
	float radius;


};

