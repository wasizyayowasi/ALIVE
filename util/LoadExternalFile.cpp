#include "LoadExternalFile.h"
#include "util/InputState.h"
#include "util/ObjectData.h"

#include<iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

using json = nlohmann::json;
using namespace std;

LoadExternalFile::LoadExternalFile(bool continuation)
{
	LoadPlayerInfo("player");
	LoadObjectData("data/objData/temp.pos");
	if (continuation) {
		LoadSaveDataInfo("saveData");
	}
}

VECTOR LoadExternalFile::DegreesToRadians(VECTOR rot)
{
	rot.x = rot.x * DX_PI_F / 180.0f;
	rot.y = rot.y * DX_PI_F / 180.0f;
	rot.z = rot.z * DX_PI_F / 180.0f;

	return rot;
}

LoadExternalFile::~LoadExternalFile()
{
	RewritePlayerInfo();
}

//�Z�[�u�f�[�^�̏����o��
void LoadExternalFile::SaveDataRewriteInfo(VECTOR pos, int num)
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

//�v���C���[�̃X�e�[�^�X����ǂݍ���
void LoadExternalFile::LoadPlayerInfo(const char* const filename)
{
	string path = "data/jsonFile/";
	path += filename;
	path += ".json";

	ifstream ifs(path.c_str());
	assert(ifs);

	json json_;
	ifs >> json_;

	player.jumpPower = json_["jumpPower"];
	player.runningJumpPower = json_["runningJumpPower"];
	player.rotSpeed = json_["rotSpeed"];
	player.walkSpeed = json_["walkSpeed"];
	player.runningSpeed = json_["runningSpeed"];

	int i = 0;
	for (auto& animNo : json_["animNo"]) {
		player.animNo_[i] = animNo;
		i++;
	}

}

//�Z�[�u�f�[�^��ǂݍ���
void LoadExternalFile::LoadSaveDataInfo(const char* const filename)
{
	string path = "data/jsonFile/";
	path += filename;
	path += ".json";

	ifstream ifs(path.c_str());
	assert(ifs);

	json json;
	ifs >> json;

	data.checkPoint.x = json["checkPointX"];
	data.checkPoint.y = json["checkPointY"];
	data.checkPoint.z = json["checkPointZ"];
	data.totalDeathNum = json["totalDeath"];

}

//�I�u�W�F�N�g�̃|�W�V������ǂݍ���
void LoadExternalFile::LoadObjectData(const char* const filename)
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
		loadObjInfo_[info.name].push_back(info);

	}

	//�t�@�C�������
	FileRead_close(dataHandle);

	//�ǂݎ�����f�[�^�̉�]����x���@����ʓx�@�ւƕϊ�����
	for (auto& obj : loadObjInfo_) {
		for (auto& objSecond : obj.second) {
			objSecond.rot = DegreesToRadians(objSecond.rot);
		}
	}

}

//�v���C���[�̃X�e�[�^�X�������o��
void LoadExternalFile::RewritePlayerInfo()
{
	json player = {
	   {"name","player"},
	   {"jumpPower",10.0f},
	   {"runningJumpPower",8.0f},
	   {"rotSpeed",15.0f},
	   {"walkSpeed",3.0f},
	   {"runningSpeed",7.0f},
	   {"animNo", json::array({ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15})},
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

