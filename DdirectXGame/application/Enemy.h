#pragma once
#include"Object3d.h"
#include"SplineCurve.h"

class Enemy : public Object3d
{
public:

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="points">�ړ��p�X�v���C���Ȑ��̍��W�z��</param>
	void Initialize(std::vector<Vector3>& points);

	void Update();

	
	void Draw();

	//�X�|�[��
	void Spawn();

private:

	//����
	SplineCurve moveLine;

	bool isAlive = false;

};

