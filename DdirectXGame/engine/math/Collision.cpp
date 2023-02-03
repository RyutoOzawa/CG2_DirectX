#include "Collision.h"
#include<cmath>

bool Collision::ColSphereToPlane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
    //座標系の頂点から球の中心座標への距離
    float distV = sphere.pos.dot(plane.normal);
    //平面の原点距離を減算することで、平面と球の中心との距離が出る
    float dist = distV - plane.distance;
    //距離の絶対値が半径より大きければあたっていない
    if (fabsf(dist) > sphere.radius)return false;

    //疑似交点を計算
    if (inter) {
        //平面上の最近接点を、疑似交点とする
        *inter = -dist * plane.normal + sphere.pos;
    }

    return true;
}
