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

	static LoadExternalFile& getInstance(bool continuation) {
		static LoadExternalFile instance(continuation);
		return instance;
	}

	ReadPlayerInfo getPlayerInfo() { return player; }
	SaveData getSaveData() { return data; }

	void saveDataRewriteInfo(VECTOR pos, int num);

private:

	void loadPlayerInfo(const char* filename);
	void loadSaveDataInfo(const char* filename);

	void rewritePlayerInfo();

	LoadExternalFile(bool temp);

	ReadPlayerInfo player;
	SaveData data;

};

