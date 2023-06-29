#pragma once
#include<string>
#include"json.hpp"


class JsonLoader
{
public://サブクラス

	//レベルデータ格納構造体
	struct LevelData {
		int num = 0;
	};

public://静的メンバ変数

	
	static const std::string defaultBaseDirectory;


	void LoadJsonFile(const std::string& filename);

};

