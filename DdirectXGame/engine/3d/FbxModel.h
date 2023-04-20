#pragma once
#include<string>

struct Node {

};

class FbxModel
{
public:
	//フレンドクラス
	friend class FbxLoader;
private:
	//モデル名
	std::string name;
};

