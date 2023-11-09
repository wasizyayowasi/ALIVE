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

class ExternalFile
{
public:

	~ExternalFile();

	static ExternalFile& GetInstance() {
		static ExternalFile instance;
		return instance;
	}

	//�ǂݍ���
	void LoadFile(bool isLood);

	/// <summary>
	/// �v���C���[�Ɋւ�������擾����
	/// </summary>
	/// <returns>playerData</returns>
	PlayerInfo GetPlayerInfo() { return player_; }

	/// <summary>
	/// savedata���擾����
	/// </summary>
	/// <returns>saveData</returns>
	SaveData GetSaveData() { return data_; }

	/// <summary>
	/// �I�u�W�F�N�g�̔z�u�����擾
	/// </summary>
	/// <returns>�z�u�f�[�^</returns>
	LoadObjectInfo GetGimmickInfo(const char* const name);
	
	/// <summary>
	/// �I�u�W�F�N�g�̔z�u�A��]���A�g�k�����擾����
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̔z�u�f�[�^�Ȃǂ����ׂĂ܂Ƃ߂��ϐ�</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetLoadObjectInfo() { return loadObjInfo_; }

	/// <summary>
	/// �w�肵�����O�̃I�u�W�F�N�g�z�u�f�[�^��Ԃ�
	/// </summary>
	/// <param name="name">�擾�������I�u�W�F�N�g�̖��O</param>
	/// <returns>�I�u�W�F�N�g�̃f�[�^</returns>
	std::list<LoadObjectInfo> GetSpecifiedInfo(const char* const name);

	/// <summary>
	/// �Z�[�u�f�[�^�̏����o��
	/// </summary>
	/// <param name="pos">�|�W�V����</param>
	/// <param name="num">���S��</param>
	void SaveDataRewriteInfo(VECTOR pos, int num);

	//�Z�[�u�f�[�^������������
	void ClearSaveData();

private:

	//�v���C���[�̃X�e�[�^�X�Ɋւ������ǂݍ���
	void LoadPlayerInfo(const char* const filename);
	//�Z�[�u�f�[�^��ǂݍ���
	void LoadSaveDataInfo(const char* const filename);
	//�I�u�W�F�N�g�̔z�u����ǂݍ���
	void LoadObjectData(const char* const filename, std::unordered_map<std::string, std::list<LoadObjectInfo>>& dataTable);

	//�v���C���[�̃X�e�[�^�X���������o��
	void RewritePlayerInfo();

	ExternalFile();

	//�x���@����ʓx�@
	VECTOR DegreesToRadians(VECTOR rot);

private:

	PlayerInfo player_;
	SaveData data_;

	ExternalFile(const ExternalFile&) = delete;
	void operator = (const ExternalFile&) = delete;

	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadObjInfo_;
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadGimmickInfo_;

};

