#pragma once
#include "DxLib.h"

#include<unordered_map>
#include<string>

/// <summary>
/// SE、BGMの管理を行うシングルトンクラス
/// </summary>
class SoundManager
{
public:

	~SoundManager();

	static SoundManager& getInstance() {
		static SoundManager instance;
		return instance;
	}

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
	int volumeBGM_ = 128;

	std::unordered_map<std::string, int> nameAndHandleTable_;
	int load2DSoundFile(const char* fileName);
	int load3DSoundFile(const char* fileName);

	SoundManager();

	SoundManager(const SoundManager&) = delete;
	void operator = (const SoundManager&) = delete;

	void loadSoundConfig();

};

