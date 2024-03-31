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

		//先頭文字取得
		char firstStr = line.data()[0];
		//先頭文字が'#'か終端文字(改行)なら無視
		if (firstStr == '#' || firstStr == '\0') {

		}else {

			//スポーンするフレ―ム
			line_stream >> newEnemyData.spawnTime;
			//退避までのフレーム
			line_stream >> newEnemyData.leaveTime;
			//カンマを飛ばしながら座標を読む
			line_stream >> newEnemyData.spawnPos.x;
			line_stream.ignore();
			line_stream >> newEnemyData.spawnPos.y;

			//出来た配置データを配列に挿入
			enemyData.push_back(newEnemyData);
		}

	}

	//ファイルを閉じる
	file.close();

	return enemyData;
}
