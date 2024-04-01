#pragma once
//当たり判定識別用ファイル

/// <summary>
/// 衝突判定用関連の型定義
/// </summary>

enum CollisionShapeType {
	SHAPE_UNKNOWN = -1,//未設定

	COLLISIONSHAPE_SPHERE,//球
	COLLISIONSHAPE_RAY,//レイ(半直線)
};

