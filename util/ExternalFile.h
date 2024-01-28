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

	/// <summary>
	/// �t�@�C�����܂Ƃ߂ēǂݍ���
	/// </summary>
	void LoadFile();

	/// <summary>
	/// �z�u�f�[�^��ǂݍ���
	/// </summary>
	void LoadArrangementData();

	/// <summary>
	/// �Z�[�u�f�[�^��ǂݍ���
	/// </summary>
	void LoadSaveData();

	/// <summary>
	/// �t�@�C����ǂݍ���
	/// </summary>
	void LoadFileHandle(std::string name);

	/// <summary>
	/// ����̃M�~�b�N�̔z�u�����擾����
	/// </summary>
	/// <param name="name">�擾�������I�u�W�F�N�g�̖��O</param>
	/// <returns></returns>
	LoadObjectInfo GetSpecifiedGimmickInfo(VECTOR objPos, const char* const name);

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
	LoadObjectInfo GetSpecifiedInfo(const char* const stage,const char* const name);

	/// <summary>
	/// �G�l�~�[�̔z�u�f�[�^���擾����
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <returns>�z�u�f�[�^</returns>
	LoadObjectInfo GetEnemyInfo(VECTOR playerPos);

	/// <summary>
	/// �I�u�W�F�N�g���폜����|�C���g�̔z�u�f�[�^���擾����
	/// </summary>
	/// <param name="pos">�v���C���[�̃|�W�V����</param>
	/// <param name="name"></param>
	/// <returns>�z�u�f�[�^</returns>
	LoadObjectInfo GetDeleteObjInfo(VECTOR pos, const char* const name);

	/// <summary>
	/// �`���[�g���A����\������|�C���g�̔z�u�f�[�^���擾����
	/// </summary>
	/// <param name="pos">�v���C���[�̃|�W�V����</param>
	/// <returns>�z�u�f�[�^</returns>
	LoadObjectInfo GetTutorialObjInfo(VECTOR pos);

	/// <summary>
	/// �Z�[�u�f�[�^���폜����
	/// </summary>
	void ClearSaveData();

	/// <summary>
	/// �Z�[�u�f�[�^�̏����o��
	/// </summary>
	/// <param name="pos">�|�W�V����</param>
	/// <param name="num">���S��</param>
	void SaveDataRewriteInfo(VECTOR pos, int num);

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
	/// ���C���X�e�[�W�I�u�W�F�N�g�̔z�u�A��]���A�g�k�����擾����
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̔z�u�f�[�^�Ȃǂ��܂Ƃ߂��ϐ�</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetLoadMainStageObjectInfo() { return loadMainStageObjInfo_; }

	/// <summary>
	/// �I�[�v�j���O�X�e�[�W�I�u�W�F�N�g�̔z�u�A��]���A�g�k�����擾����
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̔z�u�f�[�^�Ȃǂ��܂Ƃ߂��ϐ�</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetLoadOpeningStageObjectInfo() { return loadOpeningStageObjInfo_; }

	/// <summary>
	/// �J�����̍��W�f�[�^���擾����
	/// </summary>
	/// <param name="name">���O</param>
	/// <returns>���W�f�[�^</returns>
	VECTOR GetCameraTargetPos(std::string name);

	/// <summary>
	/// �w��UI�̔z�u���W���擾����
	/// </summary>
	/// <param name="name">�擾������UI�̖��O</param>
	/// <returns>���W</returns>
	VECTOR GetUIPos(std::string name);
private:

	/// <summary>
	/// �v���C���[�̃X�e�[�^�X���������o�� 
	/// </summary>
	void RewritePlayerInfo();

	/// <summary>
	/// �v���C���[�̃X�e�[�^�X�Ɋւ������ǂݍ���
	/// </summary>
	/// <param name="filename">�t�@�C���̖��O</param>
	void LoadPlayerInfo(const char* const filename);

	/// <summary>
	/// �Z�[�u�f�[�^��ǂݍ���
	/// </summary>
	/// <param name="filename">�t�@�C���̖��O</param>
	void LoadSaveDataInfo(const char* const filename);

	/// <summary>
	/// �I�u�W�F�N�g�̔z�u����ǂݍ���
	/// </summary>
	/// <param name="filename">�t�@�C���̖��O</param>
	/// <param name="dataTable">�f�[�^�e�[�u��</param>
	void LoadObjectData(std::string name, std::unordered_map<std::string, std::list<LoadObjectInfo>>& dataTable);


private:

	ExternalFile();

	ExternalFile(const ExternalFile&) = delete;
	void operator = (const ExternalFile&) = delete;

	PlayerInfo player_ = {};		//�v���C���[�̃X�e�[�^�X���
	SaveData data_ = {};			//�Z�[�u�f�[�^���

	std::unordered_map<std::string, int> loadFile_;										//���[�h�����t�@�C��
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadMainStageObjInfo_;	//���C���X�e�[�W�I�u�W�F�N�g�̔z�u�f�[�^
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadOpeningStageObjInfo_;//�I�[�v�j���O�X�e�[�W�I�u�W�F�N�g�̔z�u�f�[�^
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadGimmickInfo_;		//�M�~�b�N�̔z�u�f�[�^	
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadCameraGimmickInfo_;	//�J�����M�~�b�N�̔z�u�f�[�^		
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadEnemyInfo_;			//�G�̔z�u�f�[�^
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadTutorialInfo_;		//�`���[�g���A���|�C���g�̔z�u�f�[�^	
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadDeleteObjInfo_;		//�폜�|�C���g�̔z�u�f�[�^	
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadCameraPosInfo_;		//�J�����̔z�u�f�[�^	
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadUIInfo_;				//UI�̔z�u�f�[�^	
};

