#pragma once
#include"Vector2.h"
#include<vector>
#include<string>

//敵の配置データ
struct EnemyData {
	uint16_t spawnTime = 0;
	uint16_t leaveTime = 0;
	Vector2 spawnPos = {0,0};
};

class EnemyLoader
{
public:

	/// <summary>
	/// 敵の配置データ読み込み
	/// </summary>
	/// <param name="textfilename"></param>
	/// <returns></returns>
	static std::vector<EnemyData> LoadEnemyData(const std::string& textfilename);


private:

	//敵の配置情報配列
	//std::vector<EnemyData> enemyData;

};

