#pragma once
#include"Object3d.h"
#include"Model.h"
#include"Vector3.h"

class PlayerBullet : public Object3d
{
public:

	//èâä˙âª
	void Initialize(Model* model,const Vector3& position,const Vector3& velocity);

	void Update();

private:

	//ë¨ìx
	Vector3 velocity;

};

