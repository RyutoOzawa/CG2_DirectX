#pragma once
#include<string>
#include"json.hpp"
#include"Object3d.h"
#include"Vector3.h"
#include<memory>

//レベルデータ格納構造体
struct LevelData {

	//オブジェクトのデータ
	struct ObjectData {
		//ファイル名
		std::string filename;
		//平行移動
		Vector3 translation;
		//回転
		Vector3 rotation;
		//スケーリング
		Vector3 scaling;

		//親の名前
		std::string parentName;
		//名前
		std::string name;
	};

	//オブジェクトデータ配列
	std::vector<ObjectData>objects;


};

class JsonLoader
{
public://サブクラス



public://静的メンバ変数

	static const std::string defaultBaseDirectory;

	//jsonファイル読み込み
	static LevelData LoadJsonFile(const std::string& filename);

	//レベルデータからオブジェクト生成
	static void CreateObjectFromLevelData(const LevelData& levelData, std::vector<std::unique_ptr<Object3d>>& objects, std::vector<std::unique_ptr<Model>>& models);

private:
	//オブジェクト走査用再帰関数
	static void ScanObjects(const std::string& dataName, LevelData& levelData, nlohmann::json& daserialized, std::string parentName = "");

};