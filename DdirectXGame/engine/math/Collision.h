#pragma once

#include"CollisionPrimitive.h"

/// <summary>
/// �����蔻��w���p�[�N���X
/// </summary>
class Collision
{
public:

	/// <summary>
	/// ���ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="plane">����</param>
	/// <param name="inter">��_(���ʏ�ł̐ړ_)</param>
	/// <returns>�������Ă邩�ǂ���</returns>
	static bool ColSphereToPlane(const Sphere& sphere, const Plane& plane,
		Vector3* inter = nullptr);

};

