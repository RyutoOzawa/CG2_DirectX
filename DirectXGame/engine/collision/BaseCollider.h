#pragma once

#include"CollisionTypes.h"
#include"Object3d.h"
#include"CollisionInfo.h"


/// <summary>
/// コライダー基底クラス
/// </summary>
class BaseCollider
{
public://フレンドクラス
	friend class CollisionManager;

public:
	BaseCollider() = default;

	//仮想デストラクタ
	virtual ~BaseCollider() = default;

	inline void SetObject(Object3d* object) {
		this->object3d = object;
	}

	inline Object3d* GetObject3d() { return object3d; }

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	//形状タイプ取得
	inline CollisionShapeType GetShapeType() { return shapeType; }

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	inline void OnCollision(const CollisionInfo& info) {
		object3d->OnCollision(info);
	}

	inline void SetAttribute(unsigned short attribute_) { attribute = attribute_; }
	inline void AddAttribute(unsigned short attribute_) { attribute |= attribute_; }
	inline void RemoveAttribute(unsigned short attribute_) { attribute &= !attribute_; }

protected:
	Object3d* object3d = nullptr;
	//形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;

	//当たり判定属性
	unsigned short attribute = 0b1111111111111111;

};

