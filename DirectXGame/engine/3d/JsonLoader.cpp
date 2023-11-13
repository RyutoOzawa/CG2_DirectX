#include "JsonLoader.h"
#include<iostream>
#include<fstream>
#include<cassert>
#include"Util.h"

const std::string JsonLoader::defaultBaseDirectory = "Resources/Json/";

LevelData JsonLoader::LoadJsonFile(const std::string& filename)
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
	LevelData leveldata;

	//"objects"�̑S�I�u�W�F�N�g�𑖍�

	ScanObjects("objects", leveldata, daserialized);

	return leveldata;
}

void JsonLoader::CreateObjectFromLevelData(const LevelData& levelData, std::vector<std::unique_ptr<Object3d>>& objects, std::vector<std::unique_ptr<Model>>& models)
{

	//���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	for (auto& objectData : levelData.objects) {
		//�I�u�W�F�N�g�̃t�@�C�������烂�f������
		std::unique_ptr<Model> newModel = nullptr;
		newModel = Model::CreateModel(objectData.filename);

		//3d�I�u�W�F�N�g����
		std::unique_ptr<Object3d> newObject = nullptr;
		newObject = std::make_unique<Object3d>();
		newObject->Initialize();
		//�e�I�u�W�F�N�g���
		Vector3 pos, rot, scale;
		newObject->position = objectData.translation;
		newObject->rotation = objectData.rotation;
		newObject->scale = objectData.scaling;
		newObject->SetModel(newModel.get());
		//�e�̖��O���o�^����Ă���Ȃ玩������̔z����������Đe�I�u�W�F�N�g�Ƃ��ēo�^
		if (objectData.parentName != "") {
			int current = 0;
			for (auto& parentData : levelData.objects) {
				//�o�^����Ă���e����for���̖��O����v������for���I���
				if (parentData.name == objectData.parentName) {
					newObject->parent = objects[current].get();
					break;
				}
				//�I�u�W�F�N�g�z��̗v�f��i�߂�
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

	//�f�[�^���̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : daserialized[dataName]) {
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		std::string name = object["name"].get<std::string>();

		//��ނ��Ƃ̏���
		//MESH
		if (type.compare("MESH") == 0) {
			//�v�f�ǉ�
			leveldata.objects.emplace_back(LevelData::ObjectData{});
			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::ObjectData& objectData = leveldata.objects.back();

			if (object.contains("file_name")) {
				//�t�@�C����
				objectData.filename = object["file_name"];
			}

			//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json transform = object["transform"];
			//���s�ړ�
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = -(float)transform["translation"][1];
			//��]�p
			objectData.rotation.x = -(float)transform["rotation"][0];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = -(float)transform["rotation"][1];

			objectData.rotation.x = objectData.rotation.x * (float)PI / 180.0f;
			objectData.rotation.y = objectData.rotation.y * (float)PI / 180.0f;
			objectData.rotation.z = objectData.rotation.z * (float)PI / 180.0f;

			//�X�P�[�����O
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];

			objectData.name = name;

			//�e������Ȃ�e�̖��O��o�^
			if (parentName != "") {
				objectData.parentName = parentName;
			}

			//�R���C�_�[�̃p�����[�^�ǂݍ���

		}


		//�ċA����(�I�u�W�F�N�g�������ċA�֐��ɂ��čċA�Ăяo���Ŏ}�𑖍�����)
		if (object.contains("children")) {
			//std::string childName = object["children"].get<std::string>();
			ScanObjects("children", leveldata, object, name);
		}
	}
}