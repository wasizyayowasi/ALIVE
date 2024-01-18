#include "ExternalFile.h"
#include "Util.h"
#include "InputState.h"
#include "../object/ObjectData.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

using json = nlohmann::json;
using namespace std;

ExternalFile::ExternalFile()
{
	LoadFile();
}

ExternalFile::~ExternalFile()
{
	RewritePlayerInfo();
}

void ExternalFile::LoadFile()
{
	LoadPlayerInfo("player");
	LoadObjectData("data/objData/obj.pos",loadObjInfo_);
	LoadObjectData("data/objData/Enemy.pos",loadEnemyInfo_);
	LoadObjectData("data/objData/delete.pos",loadDeleteObjInfo_);
	LoadObjectData("data/objData/tutorial.pos", loadTutorialInfo_);
	LoadObjectData("data/objData/gimmick.pos",loadGimmickInfo_);
	LoadObjectData("data/objData/cameraGimmick.pos", loadCameraGimmickInfo_);
}

void ExternalFile::LoadSaveData()
{
	LoadSaveDataInfo("saveData");
}

LoadObjectInfo ExternalFile::GetSpecifiedGimmickInfo(VECTOR objPos, const char* const name)
{

	LoadObjectInfo info = {};

	//�ԍ��ƋL���𔲂������O���擾����
	std::string gimmickName = StrUtil::GetStringWithPartsAfterTheSymbolDeleted(name, ".");

	//�����̖��O��loadGimmickInfo_�̔z��̒��Ŗ��O����v������̂�T��
	//��v�����z��̏����擾����
	for (auto& specifiedObj : loadGimmickInfo_[gimmickName]) {
		if (specifiedObj.name != name) {
			continue;
		}
		info = specifiedObj;
	}

	//�����o��������Ԃ�
	return info;
}

LoadObjectInfo ExternalFile::GetCameraGimmickInfo(VECTOR playerPos, const char* const name)
{

	float minDistance = 10000.0f;
	LoadObjectInfo info = {};

	for (auto& data : loadCameraGimmickInfo_[name]) {
		float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(data.pos, playerPos);
		if (minDistance > distanceSize) {
			minDistance = distanceSize;
			info = data;
		}
	}
	return info;
}

std::list<LoadObjectInfo> ExternalFile::GetSpecifiedInfo(const char* const name)
{

	for (auto& obj : loadObjInfo_) {
		if (obj.first == name) {
			return loadObjInfo_[name];
		}
	}

	for (auto& gimmick : loadGimmickInfo_) {
		if (gimmick.first == name) {
			return loadGimmickInfo_[name];
		}
	}

	return std::list<LoadObjectInfo>();
}

LoadObjectInfo ExternalFile::GetEnemyInfo(VECTOR playerPos)
{
	LoadObjectInfo info = {};
	float distanceSize = 0.0f;
	float minSize = 5000.0f;

	for (auto& list : loadEnemyInfo_) {
		for (auto& enemy : list.second) {

			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(enemy.pos, playerPos);

			if (distanceSize < minSize) {
				info = enemy;
			}
		}
	}

	return info;
}

LoadObjectInfo ExternalFile::GetDeleteObjInfo(VECTOR pos, const char* const name)
{
	LoadObjectInfo info = {};
	VECTOR distance = {};
	float distanceSize = 0.0f;
	float min = 10000.0f;

	for (auto deleteObj : loadDeleteObjInfo_[name]) {
		distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(deleteObj.pos, pos);
		if (min > distanceSize) {
			min = distanceSize;
			info = deleteObj;
		}
	}

	if (info.name == "") {
		info.pos.x = 10000000.0f;
	}

	return info;
}

LoadObjectInfo ExternalFile::GetTutorialObjInfo(VECTOR pos)
{

	LoadObjectInfo info = {};
	float distanceSize = 0.0f;
	float min = 10000.0f;

	for (auto list : loadTutorialInfo_) {
		for (auto tutorialObj : list.second) {
			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(tutorialObj.pos, pos);

			if (min > distanceSize) {
				min = distanceSize;
				info = tutorialObj;
			}
		}
	}

	return info;
}

void ExternalFile::ClearSaveData()
{
	data_.checkPoint = VGet(0, 0, 0);
	data_.totalDeathNum = 0;
}

//�Z�[�u�f�[�^�̏����o��
void ExternalFile::SaveDataRewriteInfo(VECTOR pos, int num)
{
	json saveData = {
		{"name","saveData"},
		{"checkPointX",pos.x},
		{"checkPointY",pos.y},
		{"checkPointZ",pos.z},
		{"totalDeath",num},
	};

	string filename = saveData["name"];
	string extension = ".json";
	filename += extension;
	filename = "data/jsonFile/" + filename;

	ofstream writeing_file;
	writeing_file.open(filename, ios::out);
	writeing_file << saveData.dump() << std::endl;
	writeing_file.close();

}

//�v���C���[�̃X�e�[�^�X�������o��
void ExternalFile::RewritePlayerInfo()
{
	//�v���C���[�̃X�e�[�^�X���܂Ƃ߂�
	json player = {
	   {"name","player"},
	   {"jumpPower",10.0f},
	   {"runningJumpPower",8.0f},
	   {"rotSpeed",15.0f},
	   {"walkSpeed",3.0f},
	   {"runningSpeed",7.0f},
	};

	//�����o���ꏊ�̎w��
	string filename = player["name"];
	string extension = ".json";
	filename += extension;
	filename = "data/jsonFile/" + filename;

	//�o��
	ofstream writeing_file;
	writeing_file.open(filename, ios::out);
	writeing_file << player.dump() << std::endl;
	writeing_file.close();

}

//�v���C���[�̃X�e�[�^�X����ǂݍ���
void ExternalFile::LoadPlayerInfo(const char* const filename)
{
	string path = "data/jsonFile/";
	path += filename;
	path += ".json";

	ifstream ifs(path.c_str());
	assert(ifs);

	json json_;
	ifs >> json_;

	player_.jumpPower = json_["jumpPower"];
	player_.runningJumpPower = json_["runningJumpPower"];
	player_.rotSpeed = json_["rotSpeed"];
	player_.walkSpeed = json_["walkSpeed"];
	player_.runningSpeed = json_["runningSpeed"];

	ifs.close();

}

//�Z�[�u�f�[�^��ǂݍ���
void ExternalFile::LoadSaveDataInfo(const char* const filename)
{
	string path = "data/jsonFile/";
	path += filename;
	path += ".json";

	ifstream ifs(path.c_str());
	assert(ifs);

	json json;
	ifs >> json;

	data_.checkPoint.x = json["checkPointX"];
	data_.checkPoint.y = json["checkPointY"];
	data_.checkPoint.z = json["checkPointZ"];
	data_.totalDeathNum = json["totalDeath"];

	ifs.close();

}

//�I�u�W�F�N�g�̃|�W�V������ǂݍ���
void ExternalFile::LoadObjectData(const char* const filename, std::unordered_map<std::string, std::list<LoadObjectInfo>>& dataTable)
{
	//�t�@�C���̃��[�h
	auto dataHandle = FileRead_open(filename);

	//�f�[�^���̎擾
	int dataNum = 0;
	int result = FileRead_read(&dataNum, sizeof(dataNum), dataHandle);
	assert(result != -1);

	for (int i = 0; i < dataNum; i++) {

		LoadObjectInfo info;

		//���O�̕����񐔂�ǂݎ��
		uint8_t nameSize = 0;
		result = FileRead_read(&nameSize, sizeof(nameSize), dataHandle);
		assert(result != -1);

		//���O���̂��̂�ǂݎ��
		info.name.resize(nameSize);
		result = FileRead_read(info.name.data(), sizeof(char) * nameSize, dataHandle);
		assert(result != -1);

		//���W�f�[�^��info.pos���ǂݎ��
		result = FileRead_read(&info.pos, sizeof(info.pos), dataHandle);
		assert(result != -1);

		//��]����info.rot���ǂݎ��
		result = FileRead_read(&info.rot, sizeof(info.rot), dataHandle);
		assert(result != -1);

		//�g�k����info.scale���ǂݎ��
		result = FileRead_read(&info.scale, sizeof(info.scale), dataHandle);
		assert(result != -1);

		std::string keyName = StrUtil::GetStringWithPartsAfterTheSymbolDeleted(info.name, ".");

		//�ǉ�
		dataTable[keyName].push_back(info);

	}

	//�t�@�C�������
	FileRead_close(dataHandle);

	//�ǂݎ�����f�[�^�̉�]����x���@����ʓx�@�ւƕϊ�����
	for (auto& obj : dataTable) {
		for (auto& objSecond : obj.second) {
			objSecond.rot = MathUtil::VECTORDegreeToRadian(objSecond.rot);
		}
	}

}


