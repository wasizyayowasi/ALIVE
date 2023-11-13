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

	~SoundManager();

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

	void SetSEVolume(unsigned int volume);
	int GetSEVolume()const;

	void SetBGMVolume(unsigned int volume);

	void SetBGMRate(float rate);
	int GetBGMVolume()const;

	void StopSE(const char* name = nullptr);
	void StopBGM();

	void SaveSoundConfig();

	/// <summary>
	/// �T�E���h�̃|�W�V�����A��������͈͂�ݒ肷��
	/// </summary>
	/// <param name="pos">�T�E���h�̃|�W�V����</param>
	/// <param name="audioRange">��������͈�</param>
	/// <param name="name">�T�E���h��</param>
	void Set3DSoundInfo(VECTOR pos,float audioRange,const char* name);
	void Set3DSoundListenerInfo(VECTOR pos, VECTOR rot);

private:
	struct SoundConfigInfo {
		char signature[4];
		float version;
		unsigned short volumeSE;
		unsigned short volumeBGM;
	};

	int volumeSE_ = 0;
	int volumeBGM_ = 0;

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

