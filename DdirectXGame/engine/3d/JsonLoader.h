#pragma once
#include<string>
#include"json.hpp"


class JsonLoader
{
public://�T�u�N���X

	//���x���f�[�^�i�[�\����
	struct LevelData {
		int num = 0;
	};

public://�ÓI�����o�ϐ�

	
	static const std::string defaultBaseDirectory;


	void LoadJsonFile(const std::string& filename);

};

