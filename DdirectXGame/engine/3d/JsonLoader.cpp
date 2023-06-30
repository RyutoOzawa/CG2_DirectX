#include "JsonLoader.h"
#include<iostream>
#include<fstream>
#include<cassert>

const std::string JsonLoader::defaultBaseDirectory = "Resources/Json/";

LevelData* JsonLoader::LoadJsonFile(const std::string& filename)
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

	ScanObjects("Objects", leveldata, daserialized);

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : daserialized["objects"]) {
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理
		//MESH
		if (type.compare("MESH") == 0) {
			//要素追加
			leveldata->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = leveldata->objects.back();

			if (object.contains("filename")) {
				//ファイル名
				objectData.filename = object["file_name"];
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json transform = object["transform"];
			//平行移動
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = -(float)transform["translation"][0];
			//回転角
			objectData.rotation.x = -(float)transform["rotation"][1];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = (float)transform["rotation"][0];
			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];

			//コライダーのパラメータ読み込み
			
		}



		//再帰処理(オブジェクト走査を再帰関数にして再帰呼び出しで枝を走査する)
		if (object.contains("children")) {
			
		}
	}
	
	return leveldata;
}

void JsonLoader::ScanObjects(const std::string& dataName, LevelData* leveldata, const nlohmann::json& daserialized)
{
	//const nlohmann::json& a = daserialized["objects"];
	//データ名の全オブジェクトを走査
	for (const nlohmann::json& object : daserialized[dataName]) {
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理
		//MESH
		if (type.compare("MESH") == 0) {
			//要素追加
			leveldata->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = leveldata->objects.back();

			if (object.contains("filename")) {
				//ファイル名
				objectData.filename = object["file_name"];
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json transform = object["transform"];
			//平行移動
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = -(float)transform["translation"][0];
			//回転角
			objectData.rotation.x = -(float)transform["rotation"][1];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = (float)transform["rotation"][0];
			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];

			//コライダーのパラメータ読み込み

		}



		//再帰処理(オブジェクト走査を再帰関数にして再帰呼び出しで枝を走査する)
		if (object.contains("children")) {

		}
	}
}
