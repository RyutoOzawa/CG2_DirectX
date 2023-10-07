#pragma once

#include"CollisionTypes.h"
#include"Object3d.h"
#include"CollisionInfo.h"


/// <summary>
/// �R���C�_�[���N���X
/// </summary>
class BaseCollider
{
public://�t�����h�N���X
	friend class CollisionManager;

public:
	BaseCollider() = default;

	//���z�f�X�g���N�^
	virtual ~BaseCollider() = default;

	inline void SetObject(Object3d* object) {
		this->object3d = object;
	}

	inline Object3d* GetObject3d() { return object3d; }

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;
	//�`��^�C�v�擾
	inline CollisionShapeType GetShapeType() { return shapeType; }

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	inline void OnCollision(const CollisionInfo& info) {
		object3d->OnCollision(info);
	}

	inline void SetAttribute(unsigned short attribute_) { attribute = attribute_; }
	inline void AddAttribute(unsigned short attribute_) { attribute |= attribute_; }
	inline void RemoveAttribute(unsigned short attribute_) { attribute &= !attribute_; }

protected:
	Object3d* object3d = nullptr;
	//�`��^�C�v
	CollisionShapeType shapeType = SHAPE_UNKNOWN;

	//�����蔻�葮��
	unsigned short attribute = 0b1111111111111111;

};

