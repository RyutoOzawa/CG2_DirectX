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

	/// <summary>
	/// �_�ƎO�p�`�̍ŋߐړ_�����߂�
	/// </summary>
	/// <param name="point">�_</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="closest">�ŋߐړ_(�o�͗p)</param>
	static void ClosestPtPointToTriangle(const Vector3& point, const Triangle& triangle, Vector3* closest);

	/// <summary>
	/// ���Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">��_(�O�p�`��̍ŋߐړ_)</param>
	/// <returns>�������Ă��邩�ǂ���</returns>
	static bool ColSphereToTriangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);

};

