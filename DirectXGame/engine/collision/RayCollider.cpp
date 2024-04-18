#include "RayCollider.h"

void RayCollider::Update()
{

	//ワールド行列から座標を抽出
	const Matrix4& matWorld = object3d->GetMatWorld();

	//レイのメンバ更新
	Ray::start.x = matWorld.m[3][0];
	Ray::start.y = matWorld.m[3][1];
	Ray::start.z = matWorld.m[3][2];
	Ray::dir = this->dir;


}
