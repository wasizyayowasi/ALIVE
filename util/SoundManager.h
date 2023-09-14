#pragma once
#include "DxLib.h"

#include<unordered_map>
#include<string>

/// <summary>
/// SE�ABGM�̊Ǘ����s���V���O���g���N���X
/// </summary>
class SoundManager
{
public:

	~SoundManager();

	static SoundManager& getInstance() {
		static SoundManager instance;
		return instance;
	}

	/// <summary>
	/// �ǂݍ��񂾃T�E���h�𗬂�
	/// </summary>
	/// <param name="name">�t�@�C����</param>
	void play(const char* name);
	void playMusic(const char* path);

	void setSEVolume(unsigned int volume);
	int getSEVolume()const;

	void setBGMVolume(unsigned int volume);

	void setBGMRate(float rate);
	int getBGMVolume()const;

	void stopSE(const char* name = nullptr);
	void stopBGM();

	void saveSoundConfig();

	/// <summary>
	/// �T�E���h�̃|�W�V�����A��������͈͂�ݒ肷��
	/// </summary>
	/// <param name="pos">�T�E���h�̃|�W�V����</param>
	/// <param name="audioRange">��������͈�</param>
	/// <param name="name">�T�E���h��</param>
	void set3DSoundInfo(VECTOR pos,float audioRange,const char* name);
	void set3DSoundListenerInfo(VECTOR pos, VECTOR rot);

private:
	struct SoundConfigInfo {
		char signature[4];
		float version;
		unsigned short volumeSE;
		unsigned short volumeBGM;
	};

	int volumeSE_ = 128;
	int volumeBGM_ = 255;

	std::unordered_map<std::string, int> nameAndHandleTable_;
	int load2DSoundSEFile(const char* fileName);
	int load2DSoundBGMFile(const char* fileName);
	int load3DSoundSEFile(const char* fileName);
	int load3DSoundBGMFile(const char* fileName);

	SoundManager();

	SoundManager(const SoundManager&) = delete;
	void operator = (const SoundManager&) = delete;

	void loadSoundConfig();

};

