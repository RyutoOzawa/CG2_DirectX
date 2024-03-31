#include "EnemyLoader.h"
#include<fstream>
#include<sstream>
#include<cassert>
using namespace std;

std::vector<EnemyData> EnemyLoader::LoadEnemyData(const std::string& textfilename)
{
	ifstream file;
	//�e�L�X�g�t�@�C�����J��
	const string directoryPath = "Resources/";
	file.open(directoryPath + textfilename);

	//�t�@�C���I�[�v���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	//�Ԃ��z�u�f�[�^
	std::vector<EnemyData> enemyData;

	//1�s���ǂ�
	string line;
	while (getline(file, line)) {
		EnemyData newEnemyData;

		//�X�g���[���ɕϊ�
		std::istringstream line_stream(line);

		//�擪�����擾
		char firstStr = line.data()[0];
		//�擪������'#'���I�[����(���s)�Ȃ疳��
		if (firstStr == '#' || firstStr == '\0') {

		}else {

			//�X�|�[������t���\��
			line_stream >> newEnemyData.spawnTime;
			//�ޔ��܂ł̃t���[��
			line_stream >> newEnemyData.leaveTime;
			//�J���}���΂��Ȃ�����W��ǂ�
			line_stream >> newEnemyData.spawnPos.x;
			line_stream.ignore();
			line_stream >> newEnemyData.spawnPos.y;

			//�o�����z�u�f�[�^��z��ɑ}��
			enemyData.push_back(newEnemyData);
		}

	}

	//�t�@�C�������
	file.close();

	return enemyData;
}
