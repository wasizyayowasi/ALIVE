#include "ExternalFile.h"
#include "util/InputState.h"
#include "util/ObjectData.h"

#include<iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

using json = nlohmann::json;
using namespace std;

ExternalFile::ExternalFile()
{
	LoadFile(true);
}

VECTOR ExternalFile::DegreesToRadians(VECTOR rot)
{
	rot.x = rot.x * DX_PI_F / 180.0f;
	rot.y = rot.y * DX_PI_F / 180.0f;
	rot.z = rot.z * DX_PI_F / 180.0f;

	return rot;
}

ExternalFile::~ExternalFile()
{
	RewritePlayerInfo();
}

void ExternalFile::LoadFile(bool isLood)
{
	if (!loadObjInfo_.empty()) {
		loadObjInfo_.clear();
	}

	if (isLood) {
		LoadSaveDataInfo("saveData");
	}

	LoadPlayerInfo("player");
//	LoadObjectData("data/objData/obj.pos",loadObjInfo_);
	LoadObjectData("data/objData/gati.pos",loadObjInfo_);
	LoadObjectData("data/objData/gimmick.pos",loadGimmickInfo_);
}

LoadObjectInfo ExternalFile::GetGimmickInfo(const char* const name)
{
	auto info = loadGimmickInfo_[name].front();
	loadGimmickInfo_[name].pop_front();
	return info;
}

std::list<LoadObjectInfo> ExternalFile::GetSpecifiedInfo(const char* const name)
{

	for (auto& obj : loadObjInfo_) {
		if (obj.first == name) {
			return loadObjInfo_[name];
		}
	}

	return std::list<LoadObjectInfo>();
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

void ExternalFile::ClearSaveData()
{
	data_.checkPoint = VGet(0, 0, 0);
	data_.totalDeathNum = 0;
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

	int i = 0;
	for (auto& animNo : json_["animNo"]) {
		player_.animNo_[i] = animNo;
		i++;
	}

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
		
		//�ǉ�
		dataTable[info.name].push_back(info);

	}

	//�t�@�C�������
	FileRead_close(dataHandle);

	//�ǂݎ�����f�[�^�̉�]����x���@����ʓx�@�ւƕϊ�����
	for (auto& obj : dataTable) {
		for (auto& objSecond : obj.second) {
			objSecond.rot = DegreesToRadians(objSecond.rot);
		}
	}

}

//�v���C���[�̃X�e�[�^�X�������o��
void ExternalFile::RewritePlayerInfo()
{
	json player = {
	   {"name","player"},
	   {"jumpPower",10.0f},
	   {"runningJumpPower",8.0f},
	   {"rotSpeed",15.0f},
	   {"walkSpeed",3.0f},
	   {"runningSpeed",7.0f},
	   //{"animNo", json::array({ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15})},
	   {"animNo", json::array({ 0,1,2,3,4,5,6,7,8,9})},
	};

	string filename = player["name"];
	string extension = ".json";
	filename += extension;
	filename = "data/jsonFile/" + filename;

	ofstream writeing_file;
	writeing_file.open(filename, ios::out);
	writeing_file << player.dump() << std::endl;
	writeing_file.close();

}
