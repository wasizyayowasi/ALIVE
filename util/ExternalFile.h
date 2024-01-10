#pragma once
#include <unordered_map>
#include <list>
#include <string>
#include <DxLib.h>
#include "../object/PlayerData.h"
#include "../object/ObjectData.h"

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
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetGimmickInfo() { return loadGimmickInfo_; }
	
	/// <summary>
	/// ����̃M�~�b�N�̔z�u�����擾����
	/// </summary>
	/// <param name="name">�擾�������I�u�W�F�N�g�̖��O</param>
	/// <returns></returns>
	LoadObjectInfo GetSpecifiedGimmickInfo(VECTOR objPos, const char* const name);

	/// <summary>
	/// �I�u�W�F�N�g�̔z�u�A��]���A�g�k�����擾����
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̔z�u�f�[�^�Ȃǂ����ׂĂ܂Ƃ߂��ϐ�</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetLoadObjectInfo() { return loadObjInfo_; }

	/// <summary>
	/// �J����������ȓ������s����(�I�u�W�F�N�g)��
	/// �ǂ��ɂ��邩�̔z�u�f�[�^���擾����
	/// </summary>
	/// <returns></returns>
	LoadObjectInfo GetCameraGimmickInfo(VECTOR playerPos,const char* const name);

	/// <summary>
	/// �w�肵�����O�̃I�u�W�F�N�g�z�u�f�[�^��Ԃ�
	/// </summary>
	/// <param name="name">�擾�������I�u�W�F�N�g�̖��O</param>
	/// <returns>�I�u�W�F�N�g�̃f�[�^</returns>
	std::list<LoadObjectInfo> GetSpecifiedInfo(const char* const name);

	LoadObjectInfo GetEnemyInfo(VECTOR playerPos);

	LoadObjectInfo GetDeleteObjInfo(VECTOR pos, const char* const name);

	LoadObjectInfo GetTutorialObjInfo(VECTOR pos);

	/// <summary>
	/// �Z�[�u�f�[�^�̏����o��
	/// </summary>
	/// <param name="pos">�|�W�V����</param>
	/// <param name="num">���S��</param>
	void SaveDataRewriteInfo(VECTOR pos, int num);

	//�Z�[�u�f�[�^������������
	void ClearSaveData();

	//���[�h�����f�[�^���폜����
	void DeleteData();

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

	PlayerInfo player_ = {};
	SaveData data_ = {};

	ExternalFile(const ExternalFile&) = delete;
	void operator = (const ExternalFile&) = delete;

	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadObjInfo_;
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadGimmickInfo_;
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadCameraGimmickInfo_;
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadEnemyInfo_;
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadTutorialInfo_;
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadDeleteObjInfo_;

};

