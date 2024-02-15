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
}

ExternalFile::~ExternalFile()
{
	RewritePlayerInfo();
}

void ExternalFile::LoadFile()
{
	//ファイルのロード
	LoadFileHandle("obj");
	LoadFileHandle("end");
	LoadFileHandle("room");
	LoadFileHandle("UIpos");
	LoadFileHandle("Enemy");
	LoadFileHandle("startPos");
	LoadFileHandle("tutorial");
	LoadFileHandle("gimmick");
	LoadFileHandle("cameraGimmick");
	LoadFileHandle("cameraPosition");
}

void ExternalFile::LoadArrangementData()
{
	LoadPlayerInfo("player");
	LoadSaveDataInfo("saveData");
	//マップに格納する
	LoadObjectData("UIpos", loadUIInfo_);
	LoadObjectData("startPos", loadStartPos_);
	LoadObjectData("tutorial", loadTutorialInfo_);
	LoadObjectData("cameraPosition", loadCameraPosInfo_);
	//マップのリストに格納する
	LoadObjectDataList("Enemy", loadEnemyInfo_);
	LoadObjectDataList("end", loadEndStageObjInfo_);
	LoadObjectDataList("gimmick", loadGimmickInfo_);
	LoadObjectDataList("obj", loadMainStageObjInfo_);
	LoadObjectDataList("room", loadOpeningStageObjInfo_);
	LoadObjectDataList("cameraGimmick", loadCameraGimmickInfo_);
}

void ExternalFile::LoadFileHandle(std::string name)
{
	//ファイルパスの生成
	std::string filepath = "data/objData/";
	filepath = filepath + name + ".pos";

	//ファイルのロード
	loadFile_[name] = FileRead_open(filepath.c_str());
	bool temp = CheckHandleASyncLoad(loadFile_[name]);
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

LoadObjectInfo ExternalFile::GetSpecifiedInfo(const char* const stage, const char* const name)
{
	LoadObjectInfo info = {};

	if (stage == "main") {
		for (auto& obj : loadMainStageObjInfo_) {
			if (obj.first == name) {
				info = loadMainStageObjInfo_[name].front();
			}
		}
	}
	else if (stage == "title") {
		for (auto& obj : loadOpeningStageObjInfo_) {
			if (obj.first == name) {
				info = loadOpeningStageObjInfo_[name].front();
			}
		}
	}
	else if (stage == "end") {
		for (auto& obj : loadEndStageObjInfo_) {
			if (obj.first == name) {
				info = loadEndStageObjInfo_[name].front();
			}
		}
	}

	return info;
}

std::list<LoadObjectInfo> ExternalFile::GetEnemyInfo(VECTOR playerPos)
{
	std::list<LoadObjectInfo> info = {};
	float distanceSize = 0.0f;
	float minSize = 3000.0f;

	for (auto& list : loadEnemyInfo_) {
		for (auto& enemy : list.second) {
			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(enemy.pos, playerPos);

			if (distanceSize < minSize) {
				info.push_back(enemy);
			}
		}
	}

	return info;
}

VECTOR ExternalFile::GetStartPos(std::string name)
{
	return loadStartPos_[name].pos;
}

LoadObjectInfo ExternalFile::GetTutorialObjInfo(VECTOR pos)
{

	LoadObjectInfo info = {};
	float distanceSize = 0.0f;
	float min = 10000.0f;

	for (auto& tutorial : loadTutorialInfo_) {
		distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(tutorial.second.pos, pos);

		if (min > distanceSize) {
			min = distanceSize;
			info = tutorial.second;
		}
	}

	return info;
}

//セーブデータの書き出し
void ExternalFile::SaveDataRewriteInfo(int num)
{
	json saveData = {
		{"name","saveData"},
		{"pastTotalDeath",pastTotalDeathNum_},
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

void ExternalFile::SetPlayerInfo(VECTOR startPos)
{
	loadMainStageObjInfo_["Player"].front().pos = startPos;
}

VECTOR ExternalFile::GetCameraTargetPos(std::string name)
{
	VECTOR pos = {};

	for (auto& data : loadCameraPosInfo_) {
		auto& keyName = data.first;
		if (keyName == name) {
			pos = data.second.pos;
		}
	}

	return pos;
}

VECTOR ExternalFile::GetUIPos(std::string name)
{
	VECTOR pos = {};

	for (auto& data : loadUIInfo_) {
		auto& keyName = data.first;
		if (keyName == name) {
			pos = data.second.pos;
		}
	}

	return pos;
}

void ExternalFile::SetDeathCount(int num)
{
	pastTotalDeathNum_.pop_front();
	pastTotalDeathNum_.push_back(num);
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
	   {"walkSpeed",5.0f},
	   {"runningSpeed",10.0f},
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

	//pastTotalDeathNum1_.push_back(11);
	//pastTotalDeathNum1_.push_back(2);
	//pastTotalDeathNum1_.push_back(8);
	//pastTotalDeathNum1_.push_back(26);
	//pastTotalDeathNum1_.push_back(5);

	for (int i = 0; i < 5; i++) {
		pastTotalDeathNum_.push_back(json["pastTotalDeath"][i]);
	}
	//totalDeathNum_ = json["totalDeath"];

	ifs.close();
}

//オブジェクトのポジションを読み込む
void ExternalFile::LoadObjectDataList(std::string name, std::unordered_map<std::string, std::list<LoadObjectInfo>>& dataTable)
{
	//読み込んだデータのハンドルを取得
	auto dataHandle = loadFile_[name.c_str()];

	//データ数の取得
	int dataNum = 0;
	int result = FileRead_read(&dataNum, sizeof(dataNum), dataHandle);
	assert(result != -1);

	for (int i = 0; i < dataNum; i++) {

		LoadObjectInfo info = {};

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


void ExternalFile::LoadObjectData(std::string name, std::unordered_map<std::string, LoadObjectInfo>& dataTable)
{
	//読み込んだデータのハンドルを取得
	auto dataHandle = loadFile_[name.c_str()];

	//データ数の取得
	int dataNum = 0;
	int result = FileRead_read(&dataNum, sizeof(dataNum), dataHandle);
	assert(result != -1);

	for (int i = 0; i < dataNum; i++) {

		LoadObjectInfo info = {};

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
		dataTable[keyName] = info;
	}

	//ファイルを閉じる
	FileRead_close(dataHandle);

	//読み取ったデータの回転率を度数法から弧度法へと変換する
	for (auto& obj : dataTable) {
		obj.second.rot = MathUtil::VECTORDegreeToRadian(obj.second.rot);
	}
}

