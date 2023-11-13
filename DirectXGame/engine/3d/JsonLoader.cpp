#include "JsonLoader.h"
#include<iostream>
#include<fstream>
#include<cassert>
#include"Util.h"

const std::string JsonLoader::defaultBaseDirectory = "Resources/Json/";

LevelData JsonLoader::LoadJsonFile(const std::string& filename)
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
	LevelData leveldata;

	//"objects"の全オブジェクトを走査

	ScanObjects("objects", leveldata, daserialized);

	return leveldata;
}

void JsonLoader::CreateObjectFromLevelData(const LevelData& levelData, std::vector<std::unique_ptr<Object3d>>& objects, std::vector<std::unique_ptr<Model>>& models)
{

	//レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData.objects) {
		//オブジェクトのファイル名からモデル生成
		std::unique_ptr<Model> newModel = nullptr;
		newModel = Model::CreateModel(objectData.filename);

		//3dオブジェクト生成
		std::unique_ptr<Object3d> newObject = nullptr;
		newObject = std::make_unique<Object3d>();
		newObject->Initialize();
		//各オブジェクト情報
		Vector3 pos, rot, scale;
		newObject->position = objectData.translation;
		newObject->rotation = objectData.rotation;
		newObject->scale = objectData.scaling;
		newObject->SetModel(newModel.get());
		//親の名前が登録されているなら自分より上の配列を検索して親オブジェクトとして登録
		if (objectData.parentName != "") {
			int current = 0;
			for (auto& parentData : levelData.objects) {
				//登録されている親名とfor文の名前が一致したらfor文終わり
				if (parentData.name == objectData.parentName) {
					newObject->parent = objects[current].get();
					break;
				}
				//オブジェクト配列の要素を進める
				current++;
			}
		}

		newObject->Update();
		models.push_back(std::move(newModel));
		objects.push_back(std::move(newObject));
	}
}

void JsonLoader::ScanObjects(const std::string& dataName, LevelData& leveldata, nlohmann::json& daserialized, std::string parentName)
{
	//const nlohmann::json& a = daserialized["objects"];

	//auto& obj = daserialized[dataName];

	//データ名の全オブジェクトを走査
	for (nlohmann::json& object : daserialized[dataName]) {
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		std::string name = object["name"].get<std::string>();

		//種類ごとの処理
		//MESH
		if (type.compare("MESH") == 0) {
			//要素追加
			leveldata.objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = leveldata.objects.back();

			if (object.contains("file_name")) {
				//ファイル名
				objectData.filename = object["file_name"];
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json transform = object["transform"];
			//平行移動
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = -(float)transform["translation"][1];
			//回転角
			objectData.rotation.x = -(float)transform["rotation"][0];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = -(float)transform["rotation"][1];

			objectData.rotation.x = objectData.rotation.x * (float)PI / 180.0f;
			objectData.rotation.y = objectData.rotation.y * (float)PI / 180.0f;
			objectData.rotation.z = objectData.rotation.z * (float)PI / 180.0f;

			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];

			objectData.name = name;

			//親がいるなら親の名前を登録
			if (parentName != "") {
				objectData.parentName = parentName;
			}

			//コライダーのパラメータ読み込み

		}


		//再帰処理(オブジェクト走査を再帰関数にして再帰呼び出しで枝を走査する)
		if (object.contains("children")) {
			//std::string childName = object["children"].get<std::string>();
			ScanObjects("children", leveldata, object, name);
		}
	}
}