#pragma once
#include <DxLib.h>
#include<unordered_map>
#include<string>

enum class SoundPlay {
	notPlaying,		//�Đ����Ă��Ȃ�
	nowPlaying,		//�Đ���
};

/// <summary>
/// SE�ABGM�̊Ǘ����s���V���O���g���N���X
/// </summary>
class SoundManager
{
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SoundManager();

	/// <summary>
	/// �C���X�^���X�쐬
	/// </summary>
	/// <returns></returns>
	static SoundManager& GetInstance() {
		static SoundManager instance;
		return instance;
	}

	/// <summary>
	/// �����t�@�C�������[�h����
	/// </summary>
	void LoadSound();

	/// <summary>
	/// �T�E���h�{�����[����ύX����
	/// </summary>
	void ChangeVolumeMem();

	/// <summary>
	/// �ǂݍ��񂾃T�E���h�𗬂�
	/// </summary>
	/// <param name="name">�t�@�C����</param>
	void PlaySE(std::string name);

	/// <summary>
	/// �ǂݍ���BGM�𗬂�
	/// </summary>
	/// <param name="name">�t�@�C����</param>
	void PlayBGM(std::string name);

	/// <summary>
	/// SE���ʂ𒲐�����
	/// </summary>
	/// <param name="volume">����</param>
	void SetSEVolume(unsigned int volume);

	/// <summary>
	/// SE�̉��ʂ��擾����
	/// </summary>
	/// <returns>����</returns>
	int GetSEVolume()const { return volumeSE_; };

	/// <summary>
	/// BGM�̉��ʂ𒲐�����
	/// </summary>
	/// <param name="volume">����</param>
	void SetBGMVolume(unsigned int volume);

	/// <summary>
	/// BGM�̉��ʂ��擾����
	/// </summary>
	/// <returns>����</returns>
	int GetBGMVolume()const { return volumeBGM_; }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="rate"></param>
	void SetBGMRate(float rate);
	
	/// <summary>
	/// SE���~�߂�
	/// </summary>
	/// <param name="name">���̃t�@�C����</param>
	void StopSE(std::string name = "");

	/// <summary>
	/// BGM���~�߂�
	/// </summary>
	void StopBGM();

	/// <summary>
	/// BGM�ASE�̉��ʂ��O���o�͂���
	/// </summary>
	void SaveSoundConfig();

	/// <summary>
	/// �T�E���h�̃|�W�V�����A��������͈͂�ݒ肷��
	/// </summary>
	/// <param name="pos">�T�E���h�̃|�W�V����</param>
	/// <param name="audioRange">��������͈�</param>
	/// <param name="name">�T�E���h��</param>
	void Set3DSoundInfo(VECTOR pos,float audioRange,std::string name);

	/// <summary>
	/// 3D�T�E���h���Đ�����ۂ̃��X�i�[�̈ʒu�����߂�
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	void Set3DSoundListenerInfo(VECTOR pos, VECTOR rot);

	/// <summary>
	/// �T�E���h���Đ��������ׂ�
	/// </summary>
	/// <returns>1:�Đ��� 1:�I��</returns>
	int CheckSoundFile(std::string name);

private:
	//�v�@12byte
	struct SoundConfigInfo {
		char signature[4];				//4byte
		float version;					//4byte
		unsigned short volumeSE;		//2byte
		unsigned short volumeBGM;		//2byte
	};

	int volumeSE_ = 255;
	int volumeBGM_ = 255;

	std::unordered_map<std::string, int> nameAndHandleTable_;

	/// <summary>
	/// 2DSE�����[�h����
	/// </summary>
	/// <param name="fileName">�g���q�A�ꏊ�����̃t�@�C���P�̖̂��O</param>
	/// <returns>���[�h�����T�E���h</returns>
	int Load2DSoundSEFile(std::string fileName);
 
	/// <summary>
	/// 2DBGM�����[�h����
	/// </summary>
	/// <param name="fileName">�g���q�A�ꏊ�����̃t�@�C���P�̖̂��O</param>
	/// <returns>���[�h�����T�E���h</returns>
	int Load2DSoundBGMFile(std::string fileName);

	/// <summary>
	/// 3DSE�����[�h����
	/// </summary>
	/// <param name="fileName">�g���q�A�ꏊ�����̃t�@�C���P�̖̂��O</param>
	/// <returns>���[�h�����T�E���h</returns>
	int Load3DSoundSEFile(std::string fileName);

	/// <summary>
	/// 3DBGM�����[�h����
	/// </summary>
	/// <param name="fileName">�g���q�A�ꏊ�����̃t�@�C���P�̖̂��O</param>
	/// <returns>���[�h�����T�E���h</returns>
	int Load3DSoundBGMFile(std::string fileName);

	SoundManager();

	SoundManager(const SoundManager&) = delete;
	void operator = (const SoundManager&) = delete;

	void LoadSoundConfig();

};

