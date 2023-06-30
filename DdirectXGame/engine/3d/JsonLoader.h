#pragma once
#include<string>
#include"json.hpp"
#include"Object3d.h"
#include"Vector3.h"

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
	};



	//オブジェクトデータ配列
	std::vector<ObjectData>objects;


};

class JsonLoader
{
public://サブクラス



public://静的メンバ変数

	static const std::string defaultBaseDirectory;

	static LevelData* LoadJsonFile(const std::string& filename);

private:
	//オブジェクト走査用再帰関数
	static void ScanObjects(const std::string& dataName,LevelData* levelData,const nlohmann::json& daserialized);

};

