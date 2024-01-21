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
	LoadObjectData("obj",loadMainStageObjInfo_);
	LoadObjectData("room",loadOpeningStageObjInfo_);
	LoadObjectData("Enemy",loadEnemyInfo_);
	LoadObjectData("delete",loadDeleteObjInfo_);
	LoadObjectData("tutorial", loadTutorialInfo_);
	LoadObjectData("gimmick",loadGimmickInfo_);
	LoadObjectData("cameraGimmick", loadCameraGimmickInfo_);
}

void ExternalFile::LoadSaveData()
{
	LoadSaveDataInfo("saveData");
}

LoadObjectInfo ExternalFile::GetSpecifiedGimmickInfo(VECTOR objPos, const char* const name)
{

	LoadObjectInfo info = {};

	//番号と記号を抜いた名前を取得する
	std::string gimmickName = StrUtil::GetStringWithPartsAfterTheSymbolDeleted(name, ".");

	//引数の名前とloadGimmickInfo_の配列の中で名前が一致するものを探す
	//一致した配列の情報を取得する
	for (auto& specifiedObj : loadGimmickInfo_[gimmickName]) {
		if (specifiedObj.name != name) {
			continue;
		}
		info = specifiedObj;
	}

	//抜き出した情報を返す
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

	for (auto& obj : loadMainStageObjInfo_) {
		if (obj.first == name) {
			return loadMainStageObjInfo_[name];
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

//プレイヤーのステータスを書き出す
void ExternalFile::RewritePlayerInfo()
{
	//プレイヤーのステータスをまとめる
	json player = {
	   {"name","player"},
	   {"jumpPower",10.0f},
	   {"runningJumpPower",8.0f},
	   {"rotSpeed",15.0f},
	   {"walkSpeed",3.0f},
	   {"runningSpeed",7.0f},
	};

	//書き出す場所の指定
	string filename = player["name"];
	string extension = ".json";
	filename += extension;
	filename = "data/jsonFile/" + filename;

	//出力
	ofstream writeing_file;
	writeing_file.open(filename, ios::out);
	writeing_file << player.dump() << std::endl;
	writeing_file.close();

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
	//ファイルパスの生成
	std::string filepath = "data/objData/";
	filepath = filepath + filename + ".pos";

	//ファイルのロード
	auto dataHandle = FileRead_open(filepath.c_str());

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

		std::string keyName = StrUtil::GetStringWithPartsAfterTheSymbolDeleted(info.name, ".");

		//追加
		dataTable[keyName].push_back(info);

	}

	//ファイルを閉じる
	FileRead_close(dataHandle);

	//読み取ったデータの回転率を度数法から弧度法へと変換する
	for (auto& obj : dataTable) {
		for (auto& objSecond : obj.second) {
			objSecond.rot = MathUtil::VECTORDegreeToRadian(objSecond.rot);
		}
	}

}


