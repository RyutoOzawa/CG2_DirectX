#include "EnemyLoader.h"
#include<fstream>
#include<sstream>
#include<cassert>
using namespace std;

std::vector<EnemyData> EnemyLoader::LoadEnemyData(const std::string& textfilename)
{
	ifstream file;
	//テキストファイルを開く
	const string directoryPath = "Resources/";
	file.open(directoryPath + textfilename);

	//ファイルオープンチェック
	if (file.fail()) {
		assert(0);
	}

	//返す配置データ
	std::vector<EnemyData> enemyData;

	//1行ずつ読む
	string line;
	while (getline(file, line)) {
		EnemyData newEnemyData;

		//ストリームに変換
		std::istringstream line_stream(line);

		line_stream >> newEnemyData.spawnTime;
		//カンマを飛ばしながら座標を読む
		line_stream >> newEnemyData.spawnPos.x;
		line_stream.seekg(1, ios_base::cur);
		line_stream >> newEnemyData.spawnPos.y;
		line_stream.seekg(1, ios_base::cur);
		line_stream >> newEnemyData.spawnPos.z;

		//出来た配置データを配列に挿入
		enemyData.push_back(newEnemyData);

	}

	//ファイルを閉じる
	file.close();

	return enemyData;
}
