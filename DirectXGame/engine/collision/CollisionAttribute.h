#pragma once
//当たり判定属性
//衝突時に属性による排斥を行うため

const unsigned short COLLISION_ATTR_LANDSHAPE =		0b100000;		//地形
const unsigned short COLLISION_ATTR_ALLIES =		0b000001;		//自陣営
const unsigned short COLLISION_ATTR_ENEMYS =		0b000010;		//敵陣営
const unsigned short COLLISION_ATTR_INVINCIBLE =	0b111111;		//無敵	