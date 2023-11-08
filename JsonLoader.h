#pragma once
#include<string>
#include"json.hpp"
#include"Object3d.h"
#include"Vector3.h"
#include<memory>

//���x���f�[�^�i�[�\����
struct LevelData {

	//�I�u�W�F�N�g�̃f�[�^
	struct ObjectData {
		//�t�@�C����
		std::string filename;
		//���s�ړ�
		Vector3 translation;
		//��]
		Vector3 rotation;
		//�X�P�[�����O
		Vector3 scaling;

		//�e�̖��O
		std::string parentName;
		//���O
		std::string name;
	};

	//�I�u�W�F�N�g�f�[�^�z��
	std::vector<ObjectData>objects;


};

class JsonLoader
{
public://�T�u�N���X



public://�ÓI�����o�ϐ�

	static const std::string defaultBaseDirectory;

	//json�t�@�C���ǂݍ���
	static LevelData LoadJsonFile(const std::string& filename);

	//���x���f�[�^����I�u�W�F�N�g����
	static void CreateObjectFromLevelData(const LevelData& levelData, std::vector<std::unique_ptr<Object3d>>& objects, std::vector<std::unique_ptr<Model>>& models);

private:
	//�I�u�W�F�N�g�����p�ċA�֐�
	static void ScanObjects(const std::string& dataName, LevelData& levelData, nlohmann::json& daserialized, std::string parentName = "");

};