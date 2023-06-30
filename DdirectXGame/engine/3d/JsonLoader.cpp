#include "JsonLoader.h"
#include<iostream>
#include<fstream>
#include<cassert>

const std::string JsonLoader::defaultBaseDirectory = "Resources/Json/";

LevelData* JsonLoader::LoadJsonFile(const std::string& filename)
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

	ScanObjects("Objects", leveldata, daserialized);

	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : daserialized["objects"]) {
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//��ނ��Ƃ̏���
		//MESH
		if (type.compare("MESH") == 0) {
			//�v�f�ǉ�
			leveldata->objects.emplace_back(LevelData::ObjectData{});
			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::ObjectData& objectData = leveldata->objects.back();

			if (object.contains("filename")) {
				//�t�@�C����
				objectData.filename = object["file_name"];
			}

			//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json transform = object["transform"];
			//���s�ړ�
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = -(float)transform["translation"][0];
			//��]�p
			objectData.rotation.x = -(float)transform["rotation"][1];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = (float)transform["rotation"][0];
			//�X�P�[�����O
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];

			//�R���C�_�[�̃p�����[�^�ǂݍ���
			
		}



		//�ċA����(�I�u�W�F�N�g�������ċA�֐��ɂ��čċA�Ăяo���Ŏ}�𑖍�����)
		if (object.contains("children")) {
			
		}
	}
	
	return leveldata;
}

void JsonLoader::ScanObjects(const std::string& dataName, LevelData* leveldata, const nlohmann::json& daserialized)
{
	//const nlohmann::json& a = daserialized["objects"];
	//�f�[�^���̑S�I�u�W�F�N�g�𑖍�
	for (const nlohmann::json& object : daserialized[dataName]) {
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//��ނ��Ƃ̏���
		//MESH
		if (type.compare("MESH") == 0) {
			//�v�f�ǉ�
			leveldata->objects.emplace_back(LevelData::ObjectData{});
			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::ObjectData& objectData = leveldata->objects.back();

			if (object.contains("filename")) {
				//�t�@�C����
				objectData.filename = object["file_name"];
			}

			//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json transform = object["transform"];
			//���s�ړ�
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = -(float)transform["translation"][0];
			//��]�p
			objectData.rotation.x = -(float)transform["rotation"][1];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = (float)transform["rotation"][0];
			//�X�P�[�����O
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];

			//�R���C�_�[�̃p�����[�^�ǂݍ���

		}



		//�ċA����(�I�u�W�F�N�g�������ċA�֐��ɂ��čċA�Ăяo���Ŏ}�𑖍�����)
		if (object.contains("children")) {

		}
	}
}
