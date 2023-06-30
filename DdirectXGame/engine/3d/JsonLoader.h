#pragma once
#include<string>
#include"json.hpp"
#include"Object3d.h"
#include"Vector3.h"

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
	};



	//�I�u�W�F�N�g�f�[�^�z��
	std::vector<ObjectData>objects;


};

class JsonLoader
{
public://�T�u�N���X



public://�ÓI�����o�ϐ�

	static const std::string defaultBaseDirectory;

	static LevelData* LoadJsonFile(const std::string& filename);

private:
	//�I�u�W�F�N�g�����p�ċA�֐�
	static void ScanObjects(const std::string& dataName,LevelData* levelData,const nlohmann::json& daserialized);

};

