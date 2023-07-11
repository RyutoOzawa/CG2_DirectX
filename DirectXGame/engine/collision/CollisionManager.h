#pragma once
#include<forward_list>
#include<list>

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

	//�����m�̓����蔻��(��������)
	//void ColSphereToSphereAttrconst (Sphere& s1, const Sphere& s2, float* distance = nullptr, Vector3* inter = nullptr);


private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager& obj) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager& obj) = delete;
	//�R���C�_�[�̃��X�g
	std::forward_list<BaseCollider*> colliders;


};

