#pragma once
#include <vector>
#include "DxLib.h"

struct ReadPlayerInfo {
	float jumpPower;
	float runningJumpPower;
	float rotSpeed;
	float walkSpeed;
	float runningSpeed;
	int animNo_[15];
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

	void LoadPlayerInfo(const char* filename);
	void LoadSaveDataInfo(const char* filename);

	void RewritePlayerInfo();

	LoadExternalFile(bool temp);

	ReadPlayerInfo player;
	SaveData data;

};

