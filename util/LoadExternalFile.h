#pragma once
#include <unordered_map>
#include <list>
#include <string>
#include "DxLib.h"
#include "PlayerData.h"
#include "ObjectData.h"

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

	/// <summary>
	/// �v���C���[�Ɋւ�������擾����
	/// </summary>
	/// <returns>playerData</returns>
	PlayerInfo GetPlayerInfo() { return player; }

	/// <summary>
	/// savedata���擾����
	/// </summary>
	/// <returns>saveData</returns>
	SaveData GetSaveData() { return data; }
	
	/// <summary>
	/// �I�u�W�F�N�g�̔z�u�A��]���A�g�k�����擾����
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̔z�u�f�[�^�Ȃǂ����ׂĂ܂Ƃ߂��ϐ�</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetLoadObjectInfo() { return loadObjInfo_; }

	/// <summary>
	/// �Z�[�u�f�[�^�̏����o��
	/// </summary>
	/// <param name="pos">�|�W�V����</param>
	/// <param name="num">���S��</param>
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

	PlayerInfo player;
	SaveData data;

	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadObjInfo_;

};

