#pragma once

#include"BaseCollider.h"
#include"CollisionPrimitive.h"

#include"Vector3.h"

class SphereCollider : public BaseCollider, public Sphere
{
public:
	SphereCollider(Vector3 offset = {0,0,0},float radius = 0):
	offset(offset),radius(radius) {
		//球形状をセット
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	inline void SetRadius(float radius) { this->radius = radius; }

private:
	//オブジェクト中心からのオフセット
	Vector3 offset;
	//半径
	float radius;


};

