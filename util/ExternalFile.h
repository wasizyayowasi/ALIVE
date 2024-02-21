#pragma once
#include "../object/PlayerData.h"
#include "../object/ObjectData.h"
#include <list>
#include <deque>
#include <string>
#include <DxLib.h>
#include <unordered_map>

//�����摜�̍\����
struct DivGraphData
{
	std::string name = {};	//���O

	int divXNum = 0;		//�摜��X�̕�����
	int divYNum = 0;		//�摜��Yno������

	int divXSize = 0;		//�摜��X�̕����T�C�Y
	int divYSize = 0;		//�摜��Y�̕����T�C�Y
};

class ExternalFile
{
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ExternalFile();

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	/// <returns></returns>
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
	/// �t�@�C����ǂݍ���
	/// </summary>
	void LoadFileHandle(std::string name);

	/// <summary>
	/// �Z�[�u�f�[�^�̏����o��
	/// </summary>
	/// <param name="num">���S��</param>
	void SaveDataRewriteInfo(int num);

	/// <summary>
	///	���f���t�@�C���p�X��ǂݍ���
	/// </summary>
	void LoadModelFilePath();

	/// <summary>
	/// �摜�̃t�@�C���p�X��ǂݍ���
	/// </summary>
	void LoadGraphFilePath();

	/// <summary>
	/// �����摜�̃t�@�C���p�X��ǂݍ���
	/// </summary>
	void LoadDivGraphFilePath();


	////////////////Getter////////////////

	/// <summary>
	/// ����̃M�~�b�N�̔z�u�����擾����
	/// </summary>
	/// <param name="name">�擾�������I�u�W�F�N�g�̖��O</param>
	/// <returns></returns>
	LoadObjectInfo GetSpecifiedGimmickInfo(const VECTOR objPos, const char* const name);

	/// <summary>
	/// �J����������ȓ������s����(�I�u�W�F�N�g)��
	/// �ǂ��ɂ��邩�̔z�u�f�[�^���擾����
	/// </summary>
	/// <returns></returns>
	LoadObjectInfo GetCameraGimmickInfo(const VECTOR playerPos, const char* const name);

	/// <summary>
	/// �w�肵�����O�̃I�u�W�F�N�g�z�u�f�[�^��Ԃ�
	/// </summary>
	/// <param name="name">�擾�������I�u�W�F�N�g�̖��O</param>
	/// <returns>�I�u�W�F�N�g�̃f�[�^</returns>
	LoadObjectInfo GetSpecifiedInfo(const char* const stage, const char* const name);

	/// <summary>
	/// �G�l�~�[�̔z�u�f�[�^���擾����
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <returns>�z�u�f�[�^</returns>
	std::list<LoadObjectInfo> GetEnemyInfo(const VECTOR playerPos);

	/// <summary>
	/// �v���C���[�̊J�n�ʒu�̃f�[�^���擾����
	/// </summary>
	/// <param name="name">�J�n�ʒu�̖��O</param>
	/// <returns>�z�u�f�[�^</returns>
	VECTOR GetStartPos(const std::string name);

	/// <summary>
	/// �`���[�g���A����\������|�C���g�̔z�u�f�[�^���擾����
	/// </summary>
	/// <param name="pos">�v���C���[�̃|�W�V����</param>
	/// <returns>�z�u�f�[�^</returns>
	LoadObjectInfo GetTutorialObjInfo(const VECTOR pos);

	/// <summary>
	/// �J�����̍��W�f�[�^���擾����
	/// </summary>
	/// <param name="name">���O</param>
	/// <returns>���W�f�[�^</returns>
	VECTOR GetCameraTargetPos(const std::string name);

	/// <summary>
	/// �w��UI�̔z�u���W���擾����
	/// </summary>
	/// <param name="name">�擾������UI�̖��O</param>
	/// <returns>���W</returns>
	VECTOR GetUIPos(const std::string name);

	/// <summary>
	/// �v���C���[�Ɋւ�������擾����
	/// </summary>
	/// <returns>playerData</returns>
	PlayerInfo GetPlayerInfo() const { return player_; }

	/// <summary>
	/// ���C���X�e�[�W�̓���ȃI�u�W�F�N�g�̔z�u�f�[�^���擾����
	/// </summary>
	/// <param name="name">���O</param>
	/// <returns>�z�u�f�[�^</returns>
	LoadObjectInfo GetMainSpecialObjectInfo(const std::string name) { return loadSpecialInfo_[name]; }

	/// <summary>
	/// �J�n�ꏊ�̖��O���擾����
	/// </summary>
	/// <returns>�J�n�ꏊ�̖��O</returns>
	const std::string& GetStartName()const { return startPosName_; }

	/// <summary>
	/// savedata���擾����
	/// </summary>
	/// <returns>�ߋ��̎��S��</returns>
	std::deque<int> GetTotalDeathNum() const { return pastTotalDeathNum_; }

	/// <summary>
	/// �I�u�W�F�N�g�̔z�u�����擾
	/// </summary>
	/// <returns>�z�u�f�[�^</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetGimmickInfo() const { return loadGimmickInfo_; }

	/// <summary>
	/// ���C���X�e�[�W�I�u�W�F�N�g�̔z�u�A��]���A�g�k�����擾����
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̔z�u�f�[�^�Ȃǂ��܂Ƃ߂��ϐ�</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetLoadMainStageObjectInfo()const { return loadMainStageObjInfo_; }

	/// <summary>
	/// �I�[�v�j���O�X�e�[�W�I�u�W�F�N�g�̔z�u�A��]���A�g�k�����擾����
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̔z�u�f�[�^�Ȃǂ��܂Ƃ߂��ϐ�</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetLoadOpeningStageObjectInfo() const { return loadOpeningStageObjInfo_; }

	/// <summary>
	/// �G���f�B���O�V�[���I�u�W�F�N�g�̔z�u�A��]���A�g�k�����擾����
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̔z�u�f�[�^�Ȃǂ��܂Ƃ߂��ϐ�</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetLoadEndingStageObjectInfo()const { return loadEndStageObjInfo_; }

	/// <summary>
	/// ���f���t�@�C���p�X���擾����
	/// </summary>
	/// <returns>�t�@�C���p�X���܂Ƃ߂��e�[�u��</returns>
	std::unordered_map<std::string, std::list<std::string>> GetModelFilePath()const { return modelFilePathInfo_; }

	/// <summary>
	/// �摜�t�@�C���p�X���擾����
	/// </summary>
	/// <returns>�t�@�C���p�X���܂Ƃ߂��e�[�u��</returns>
	std::unordered_map<std::string, std::list<std::string>> GetGraphFilePath()const { return graphFilePathInfo_; }

	/// <summary>
	/// �����f�[�^���擾����
	/// </summary>
	/// <returns>�����f�[�^���܂Ƃ߂��e�[�u��</returns>
	std::unordered_map<std::string, std::list<DivGraphData>> GetDivGraphData()const { return divGraphFilePathInfo_; }


	////////////////Setter////////////////

	/// <summary>
	/// �J�n�ꏊ�̖��O��ݒ肷��
	/// </summary>
	/// <param name="name">�J�n�ꏊ�̖��O</param>
	void SetStartName(const std::string name);

	/// <summary>
	/// ���񂾉񐔂��Z�b�g����
	/// </summary>
	/// <param name="num"></param>
	void SetDeathCount(const int num);
private:

	/// <summary>
	/// �v���C���[�̃X�e�[�^�X���������o�� 
	/// </summary>
	void RewritePlayerInfo();

	/// <summary>
	/// �v���C���[�̃X�e�[�^�X�Ɋւ������ǂݍ���
	/// </summary>
	void LoadPlayerInfo();

	/// <summary>
	/// �Z�[�u�f�[�^��ǂݍ���
	/// </summary>
	/// <param name="filename">�t�@�C���̖��O</param>
	void LoadSaveDataInfo(const char* const filename);

	/// <summary>
	/// �I�u�W�F�N�g�̔z�u����ǂݍ���list�Ɋi�[����
	/// </summary>
	/// <param name="filename">�t�@�C���̖��O</param>
	/// <param name="dataTable">�f�[�^�e�[�u��</param>
	void LoadObjectDataList(const std::string name, std::unordered_map<std::string, std::list<LoadObjectInfo>>& dataTable);

	/// <summary>
	/// �I�u�W�F�N�g�̔z�u����ǂݍ���
	/// </summary>
	/// /// <param name="filename">�t�@�C���̖��O</param>
	/// <param name="dataTable">�f�[�^�e�[�u��</param>
	void LoadObjectData(const std::string name, std::unordered_map<std::string, LoadObjectInfo>& dataTable);
private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ExternalFile();

	ExternalFile(const ExternalFile&) = delete;
	void operator = (const ExternalFile&) = delete;

	PlayerInfo player_ = {};		//�v���C���[�̃X�e�[�^�X���

	std::string startPosName_ = {};	//�J�n�ʒu�̖��O

	std::deque<int> pastTotalDeathNum_ = {};											//�ߋ�5��̎��S��

	std::unordered_map<std::string, int> loadFile_;										//���[�h�����t�@�C��
	std::unordered_map<std::string, std::list<std::string>> modelFilePathInfo_;			//���f���t�@�C���p�X���܂Ƃ߂�
	std::unordered_map<std::string, std::list<std::string>> graphFilePathInfo_;			//�摜�t�@�C���p�X���܂Ƃ߂�
	std::unordered_map<std::string, std::list<DivGraphData>>divGraphFilePathInfo_;		//�����摜�t�@�C���p�X���܂Ƃ߂�

	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadMainStageObjInfo_;	//���C���X�e�[�W�I�u�W�F�N�g�̔z�u�f�[�^
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadOpeningStageObjInfo_;//�I�[�v�j���O�X�e�[�W�I�u�W�F�N�g�̔z�u�f�[�^
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadEndStageObjInfo_;	//�I�[�v�j���O�X�e�[�W�I�u�W�F�N�g�̔z�u�f�[�^
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadGimmickInfo_;		//�M�~�b�N�̔z�u�f�[�^	
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadCameraGimmickInfo_;	//�J�����M�~�b�N�̔z�u�f�[�^		
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadEnemyInfo_;			//�G�̔z�u�f�[�^

	std::unordered_map<std::string, LoadObjectInfo>			   loadUIInfo_;				//UI�̔z�u�f�[�^	
	std::unordered_map<std::string, LoadObjectInfo>			   loadStartPos_;			//�J�n�ʒu�̃f�[�^	
	std::unordered_map<std::string, LoadObjectInfo>			   loadSpecialInfo_;		//����ȃI�u�W�F�N�g�̔z�u�f�[�^	
	std::unordered_map<std::string, LoadObjectInfo>			   loadTutorialInfo_;		//�`���[�g���A���|�C���g�̔z�u�f�[�^	
	std::unordered_map<std::string, LoadObjectInfo>			   loadCameraPosInfo_;		//�J�����̔z�u�f�[�^	
};

