#include "Collision.h"
#include<cmath>

bool Collision::ColSphereToPlane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
    //���W�n�̒��_���狅�̒��S���W�ւ̋���
    float distV = sphere.pos.dot(plane.normal);
    //���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
    float dist = distV - plane.distance;
    //�����̐�Βl�����a���傫����΂������Ă��Ȃ�
    if (fabsf(dist) > sphere.radius)return false;

    //�^����_���v�Z
    if (inter) {
        //���ʏ�̍ŋߐړ_���A�^����_�Ƃ���
        *inter = -dist * plane.normal + sphere.pos;
    }

    return true;
}
