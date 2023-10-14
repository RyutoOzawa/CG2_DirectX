#pragma once
//衝突情報
//衝突した相手の情報を格納するためのクラス

#include"Vector3.h"

class Object3d;
class BaseCollider;

/// <summary>
/// 衝突情報
/// </summary>
class CollisionInfo
{
public:
	CollisionInfo(Object3d* object_, BaseCollider* collider_, const Vector3& inter_) {
		object = object_;
		collider = collider_;
		inter = inter_;
	}

	//衝突相手のオブジェクト
	Object3d* object = nullptr;
	//衝突相手のコライダー
	BaseCollider* collider = nullptr;
	//衝突点
	Vector3 inter;
};

