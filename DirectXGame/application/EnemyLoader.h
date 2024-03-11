#pragma once
#include"Vector3.h"
#include<vector>
#include<string>

//�G�̔z�u�f�[�^
struct EnemyData {
	uint16_t spawnTime = 0;
	uint16_t leaveTime = 0;
	Vector3 spawnPos = {0,0,0};
};

class EnemyLoader
{
public:

	/// <summary>
	/// �G�̔z�u�f�[�^�ǂݍ���
	/// </summary>
	/// <param name="textfilename"></param>
	/// <returns></returns>
	static std::vector<EnemyData> LoadEnemyData(const std::string& textfilename);


private:

	//�G�̔z�u���z��
	//std::vector<EnemyData> enemyData;

};

