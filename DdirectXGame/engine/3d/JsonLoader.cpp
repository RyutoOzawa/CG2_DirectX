#include "JsonLoader.h"
#include<iostream>
#include<fstream>
#include<cassert>

const std::string JsonLoader::defaultBaseDirectory = "Resources/Json/";

void JsonLoader::LoadJsonFile(const std::string& filename)
{
	//拡張子
	const std::string extension = ".json";

	//読み込むファイルのフルパス
	const std::string fullpath = defaultBaseDirectory + filename + extension;

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	//json文字列から解凍したデータ
	nlohmann::json daserialized;

	//解凍
	file >> daserialized;

	//正しいレベルデータファイルかチェック
	assert(daserialized.is_object());
	assert(daserialized.contains("name"));
	assert(daserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = daserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	LevelData* leveldata = new LevelData();

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : daserialized["objects"]) {
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理
		//MESH
		if (type.compare("MESH") == 0) {
			//要素追加
			
		}



		//再帰処理
	}
	
}
