//球体コライダークラス
//球体の当たり判定を持つクラス
#include "SphereCollider.h"

void SphereCollider::Update()
{
	//ワールド行列から座標を抽出
	const Matrix4& matWorld = object3d->GetMatWorld();

	//球のメンバ変数を更新
	Sphere::pos.x = matWorld.m[3][0];
	Sphere::pos.y = matWorld.m[3][1];
	Sphere::pos.z = matWorld.m[3][2];
	Sphere::pos + offset;
	Sphere::radius = radius;
}
