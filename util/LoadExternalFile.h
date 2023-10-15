#pragma once
#include <unordered_map>
#include <list>
#include <string>
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

struct LoadObjectInfo {
	std::string name;	//�I�u�W�F�N�g�̖��O
	VECTOR pos;			//�|�W�V����
	VECTOR rot;			//��]��
	VECTOR scale;		//�g�k��
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
	void LoadObjectData(const char* const filename);

	//�v���C���[�̃X�e�[�^�X���������o��
	void RewritePlayerInfo();

	LoadExternalFile(bool temp);

	//�x���@����ʓx�@
	VECTOR DegreesToRadians(VECTOR rot);

private:

	ReadPlayerInfo player;
	SaveData data;

	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadObjInfo_;

};

