#include "LoadExternalFile.h"
#include "util/InputState.h"

#include<iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

using json = nlohmann::json;
using namespace std;

LoadExternalFile::LoadExternalFile(bool continuation)
{
	LoadPlayerInfo("player");
	if (continuation) {
		LoadSaveDataInfo("saveData");
	}
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
void LoadExternalFile::LoadObjectPosition(const char* const filename)
{

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

