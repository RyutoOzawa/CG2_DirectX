#include "JsonLoader.h"
#include<iostream>
#include<fstream>
#include<cassert>

const std::string JsonLoader::defaultBaseDirectory = "Resources/Json/";

void JsonLoader::LoadJsonFile(const std::string& filename)
{
	//�g���q
	const std::string extension = ".json";

	//�ǂݍ��ރt�@�C���̃t���p�X
	const std::string fullpath = defaultBaseDirectory + filename + extension;

	//�t�@�C���X�g���[��
	std::ifstream file;

	//�t�@�C�����J��
	file.open(fullpath);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	//json�����񂩂�𓀂����f�[�^
	nlohmann::json daserialized;

	//��
	file >> daserialized;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(daserialized.is_object());
	assert(daserialized.contains("name"));
	assert(daserialized["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	std::string name = daserialized["name"].get<std::string>();
	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	LevelData* leveldata = new LevelData();

	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : daserialized["objects"]) {
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//��ނ��Ƃ̏���
		//MESH
		if (type.compare("MESH") == 0) {
			//�v�f�ǉ�
			
		}



		//�ċA����
	}
	
}
