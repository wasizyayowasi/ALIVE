#pragma once
#include <DxLib.h>

#include<unordered_map>
#include<string>

/// <summary>
/// SE�ABGM�̊Ǘ����s���V���O���g���N���X
/// </summary>
class SoundManager
{
public:
	//
	~SoundManager();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	static SoundManager& GetInstance() {
		static SoundManager instance;
		return instance;
	}

	/// <summary>
	/// �ǂݍ��񂾃T�E���h�𗬂�
	/// </summary>
	/// <param name="name">�t�@�C����</param>
	void Play(const char* name);
	void PlayBGM(const char* path);

	/// <summary>
	/// SE���ʂ𒲐�����
	/// </summary>
	/// <param name="volume">����</param>
	void SetSEVolume(unsigned int volume);

	/// <summary>
	/// SE�̉��ʂ��擾����
	/// </summary>
	/// <returns>����</returns>
	int GetSEVolume()const;

	/// <summary>
	/// BGM�̉��ʂ𒲐�����
	/// </summary>
	/// <param name="volume">����</param>
	void SetBGMVolume(unsigned int volume);

	/// <summary>
	/// BGM�̉��ʂ��擾����
	/// </summary>
	/// <returns>����</returns>
	int GetBGMVolume()const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="rate"></param>
	void SetBGMRate(float rate);
	
	/// <summary>
	/// SE���~�߂�
	/// </summary>
	/// <param name="name">���̃t�@�C����</param>
	void StopSE(const char* name = nullptr);

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
	void Set3DSoundInfo(VECTOR pos,float audioRange,const char* name);

	/// <summary>
	/// 3D�T�E���h���Đ�����ۂ̃��X�i�[�̈ʒu�����߂�
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	void Set3DSoundListenerInfo(VECTOR pos, VECTOR rot);

private:
	struct SoundConfigInfo {
		char signature[4];
		float version;
		unsigned short volumeSE;
		unsigned short volumeBGM;
	};

	int volumeSE_ = 50;
	int volumeBGM_ = 50;

	std::unordered_map<std::string, int> nameAndHandleTable_;
	int Load2DSoundSEFile(const char* fileName);
	int Load2DSoundBGMFile(const char* fileName);
	int Load3DSoundSEFile(const char* fileName);
	int Load3DSoundBGMFile(const char* fileName);

	SoundManager();

	SoundManager(const SoundManager&) = delete;
	void operator = (const SoundManager&) = delete;

	void LoadSoundConfig();

};

