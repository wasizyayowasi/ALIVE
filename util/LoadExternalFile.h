#pragma once
#include <vector>
#include "DxLib.h"
#include "ObjAnimType.h"

struct ReadPlayerInfo {
	float jumpPower;
	float runningJumpPower;
	float rotSpeed;
	float walkSpeed;
	float runningSpeed;
	int animNo_[static_cast<int>(AnimType::max)];
};

struct SaveData {
	VECTOR checkPoint;
	int totalDeathNum;
};

class LoadExternalFile
{
public:

	~LoadExternalFile();

	static LoadExternalFile& GetInstance(bool continuation) {
		static LoadExternalFile instance(continuation);
		return instance;
	}

	ReadPlayerInfo GetPlayerInfo() { return player; }
	SaveData GetSaveData() { return data; }

	void SaveDataRewriteInfo(VECTOR pos, int num);

private:

	//プレイヤーのステータスに関する情報を読み込む
	void LoadPlayerInfo(const char* const filename);
	//セーブデータを読み込む
	void LoadSaveDataInfo(const char* const filename);
	//オブジェクトの配置情報を読み込む
	void LoadObjectPosition(const char* const filename);

	//プレイヤーのステータス情報を書き出す
	void RewritePlayerInfo();

	LoadExternalFile(bool temp);

	ReadPlayerInfo player;
	SaveData data;

};

