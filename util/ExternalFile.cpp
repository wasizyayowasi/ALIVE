#include "ExternalFile.h"
#include "InputState.h"
#include "ObjectData.h"
#include "Util.h"

#include <iostream>
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
	DeleteData();

	if (isLood) {
		LoadSaveDataInfo("saveData");
	}

	LoadPlayerInfo("player");
	LoadObjectData("data/objData/obj.pos",loadObjInfo_);
	LoadObjectData("data/objData/Enemy.pos",loadEnemyInfo_);
	LoadObjectData("data/objData/delete.pos",loadDeleteObjInfo_);
	LoadObjectData("data/objData/tutorial.pos", loadTutorialInfo_);
	LoadObjectData("data/objData/gimmick.pos",loadGimmickInfo_);
	LoadObjectData("data/objData/cameraGimmick.pos", loadCameraGimmickInfo_);
}

LoadObjectInfo ExternalFile::GetSpecifiedGimmickInfo(VECTOR objPos, const char* const name)
{

	LoadObjectInfo info = {};

	float minDIstance = 10000.0f;
	float distanceSize = 0.0f;

	std::string key = StrUtil::GetStringWithPartsAfterTheSymbolDeleted(name, ".");

	for (auto& specifiedObj : loadGimmickInfo_[key]) {
		if (specifiedObj.name != name) {
			continue;
		}
		distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(objPos, specifiedObj.pos);
		if (minDIstance > distanceSize) {
			minDIstance = distanceSize;
			info = specifiedObj;
		}
	}

	loadGimmickInfo_[key].remove_if([&info](LoadObjectInfo objInfo) {return VSize(objInfo.pos) == VSize(info.pos); });

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

	std::string key = StrUtil::GetStringWithPartsAfterTheSymbolDeleted(info.name, ".");

	loadEnemyInfo_[key].remove_if([&info](LoadObjectInfo objInfo) {return objInfo.pos.x == info.pos.x && objInfo.pos.y == info.pos.y && objInfo.pos.z == info.pos.z; });

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

	//loadDeleteObjInfo_[info.name].remove_if([&info](LoadObjectInfo objInfo) {return objInfo.pos.x == info.pos.x && objInfo.pos.y == info.pos.y && objInfo.pos.z == info.pos.z; });

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

//セーブデータの書き出し
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

void ExternalFile::DeleteData()
{
	if (!loadObjInfo_.empty()) {
		loadObjInfo_.clear();
	}
	if (!loadGimmickInfo_.empty()) {
		loadGimmickInfo_.clear();
	}
	if (!loadCameraGimmickInfo_.empty()) {
		loadCameraGimmickInfo_.clear();
	}
	if (!loadEnemyInfo_.empty()) {
		loadEnemyInfo_.clear();
	}
	if (!loadDeleteObjInfo_.empty()) {
		loadDeleteObjInfo_.clear();
	}
}

//プレイヤーのステータス情報を読み込む
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

//セーブデータを読み込む
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

//オブジェクトのポジションを読み込む
void ExternalFile::LoadObjectData(const char* const filename, std::unordered_map<std::string, std::list<LoadObjectInfo>>& dataTable)
{
	//ファイルのロード
	auto dataHandle = FileRead_open(filename);

	//データ数の取得
	int dataNum = 0;
	int result = FileRead_read(&dataNum, sizeof(dataNum), dataHandle);
	assert(result != -1);

	for (int i = 0; i < dataNum; i++) {

		LoadObjectInfo info;

		//名前の文字列数を読み取る
		uint8_t nameSize = 0;
		result = FileRead_read(&nameSize, sizeof(nameSize), dataHandle);
		assert(result != -1);

		//名前そのものを読み取る
		info.name.resize(nameSize);
		result = FileRead_read(info.name.data(), sizeof(char) * nameSize, dataHandle);
		assert(result != -1);

		//座標データをinfo.pos分読み取る
		result = FileRead_read(&info.pos, sizeof(info.pos), dataHandle);
		assert(result != -1);

		//回転率をinfo.rot分読み取る
		result = FileRead_read(&info.rot, sizeof(info.rot), dataHandle);
		assert(result != -1);

		//拡縮率をinfo.scale分読み取る
		result = FileRead_read(&info.scale, sizeof(info.scale), dataHandle);
		assert(result != -1);
		
		std::string keyName = StrUtil::GetStringWithPartsAfterTheSymbolDeleted(info.name,".");

		//追加
		dataTable[keyName].push_back(info);

	}

	//ファイルを閉じる
	FileRead_close(dataHandle);

	//読み取ったデータの回転率を度数法から弧度法へと変換する
	for (auto& obj : dataTable) {
		for (auto& objSecond : obj.second) {
			objSecond.rot = DegreesToRadians(objSecond.rot);
		}
	}

}

//プレイヤーのステータスを書き出す
void ExternalFile::RewritePlayerInfo()
{
	json player = {
	   {"name","player"},
	   {"jumpPower",10.0f},
	   {"runningJumpPower",8.0f},
	   {"rotSpeed",15.0f},
	   {"walkSpeed",3.0f},
	   {"runningSpeed",7.0f},
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

