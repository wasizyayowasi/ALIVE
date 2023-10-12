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

	//�v���C���[�̃X�e�[�^�X�Ɋւ������ǂݍ���
	void LoadPlayerInfo(const char* const filename);
	//�Z�[�u�f�[�^��ǂݍ���
	void LoadSaveDataInfo(const char* const filename);
	//�I�u�W�F�N�g�̔z�u����ǂݍ���
	void LoadObjectPosition(const char* const filename);

	//�v���C���[�̃X�e�[�^�X���������o��
	void RewritePlayerInfo();

	LoadExternalFile(bool temp);

	ReadPlayerInfo player;
	SaveData data;

};

