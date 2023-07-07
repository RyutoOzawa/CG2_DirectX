#pragma once
#include<forward_list>

class BaseCollider;

class CollisionManager
{
public: //静的メンバ変数
	static CollisionManager* GetInstance();

public://メンバ関数

	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="collider">コライダー</param>
	inline void AddCollider(BaseCollider* collider) { colliders.push_front(collider); }

	/// <summary>
	/// コライダーの削除
	/// </summary>
	/// <param name="collider">コライダー</param>
	inline void RemoveCollider(BaseCollider* collider) { colliders.remove(collider); }

	//すべての衝突チェック
	void CheckAllCollisions();

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager& obj) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager& obj) = delete;
	//コライダーのリスト
	std::forward_list<BaseCollider*> colliders;


};

