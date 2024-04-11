#pragma once
//レイ(半直線)コライダークラス
//レイの当たり判定用クラス

#include"BaseCollider.h"
#include"CollisionPrimitive.h"

#include"Vector3.h"

class RayCollider : public BaseCollider,public Ray
{
public:
	RayCollider(Vector3 offset_ = { 0,0,0 }) : offset(offset_) {
		//形状をセット
		shapeType = COLLISIONSHAPE_RAY;
	}


	//更新
	void Update();

	

private:

	//中心からのオフセット
	Vector3 offset;


};

