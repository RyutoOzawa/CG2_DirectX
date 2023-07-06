#pragma once
#include<forward_list>

class BaseCollider;

class CollisionManager
{
public: //�ÓI�����o�ϐ�
	static CollisionManager* GetInstance();

public://�����o�֐�

	/// <summary>
	/// �R���C�_�[�̒ǉ�
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	inline void AddCollider(BaseCollider* collider) { colliders.push_front(collider); }

	/// <summary>
	/// �R���C�_�[�̍폜
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	inline void RemoveCollider(BaseCollider* collider) { colliders.remove(collider); }

	//���ׂĂ̏Փ˃`�F�b�N
	void CheckAllCollisions();

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& oparator=(const CollisionManager&) = delete;
	//�R���C�_�[�̃��X�g
	std::forward_list<BaseCollider*> colliders;


};

