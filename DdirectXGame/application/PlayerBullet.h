#pragma once
#include"Object3d.h"
#include"Model.h"
#include"Vector3.h"

class PlayerBullet : public Object3d
{
public:

	//‰Šú‰»
	void Initialize(Model* model,const Vector3& position);

	void Update();

private:
};

