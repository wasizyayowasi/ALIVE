#include "SoundManager.h"
#include <cassert>

namespace {
    constexpr char sound_file_signature[] = "SND_";
    constexpr char sound_config_file_path[] = "./sound.conf";
    constexpr float sound_config_version = 1.0f;
}

using namespace std;

SoundManager::SoundManager() {
    loadSoundConfig();
    setSEVolume(volumeSE_);
    setBGMVolume(volumeBGM_);
}

/// <summary>
/// 2Dサウンドをロードする
/// </summary>
/// <param name="fileName">拡張子、場所抜きのファイル単体の名前</param>
/// <returns>ロードしたサウンド</returns>
int SoundManager::load2DSoundFile(const char* fileName)
{
    //ファイルパスの生成
    string path = "data/sound/SE/";
    path += fileName;
    path += ".mp3";

    //メモリに2Dサウンドをロードする
    int handle = LoadSoundMem(path.c_str());
    assert(handle >= 0);

    //配列にロードしたものを加える
    nameAndHandleTable_[fileName] = handle;

    return handle;
}

int SoundManager::load3DSoundFile(const char* fileName)
{
    //ファイルパスの生成
    string path = "data/sound/SE/";
    path += fileName;
    path += ".mp3";

    //メモリに3Dサウンドをロードする
    SetCreate3DSoundFlag(true);
    int handle = LoadSoundMem(path.c_str());
    SetCreate3DSoundFlag(false);
    assert(handle >= 0);

    //配列にロードしたものを加える
    nameAndHandleTable_[fileName] = handle;

    return handle;
}

void SoundManager::loadSoundConfig()
{
    SoundConfigInfo conf = {};
    FILE* fp = nullptr;
    fopen_s(&fp, sound_config_file_path, "rb");
    if (fp) {
        fread(&conf, sizeof(conf), 1, fp);
        fclose(fp);
        volumeBGM_ = conf.volumeBGM;
        volumeSE_ = conf.volumeSE;
    }
}

SoundManager::~SoundManager()
{
}

void SoundManager::play(const char* name)
{
    assert(nameAndHandleTable_[name] != -1);
    PlaySoundMem(nameAndHandleTable_[name], DX_PLAYTYPE_BACK);
}

void SoundManager::playMusic(const char* path)
{
    PlayMusic(path, DX_PLAYTYPE_LOOP);
    SetVolumeMusic(volumeBGM_);
}

void SoundManager::setSEVolume(unsigned int volume)
{
    for (auto& record : nameAndHandleTable_) {
        ChangeVolumeSoundMem(volume, record.second);
    }
    volumeSE_ = volume;
}

int SoundManager::getSEVolume() const
{
    return volumeSE_;
}

void SoundManager::setBGMVolume(unsigned int volume)
{
    SetVolumeMusic(volume);
    volumeBGM_ = volume;
}

void SoundManager::setBGMRate(float rate)
{
    assert(0.0f <= rate && rate <= 1.0f);
    SetVolumeMusic(static_cast<int>(static_cast<float>(volumeBGM_ * rate)));
}

int SoundManager::getBGMVolume() const
{
    return volumeBGM_;
}

void SoundManager::stopSE(const char* name)
{
    if (name != nullptr) {
        StopSoundMem(nameAndHandleTable_[name]);
    }
}

void SoundManager::stopBGM()
{
    StopMusic();
}

void SoundManager::saveSoundConfig()
{
    SoundConfigInfo conf = {};
    std::copy_n(std::begin(sound_file_signature), sizeof(char) * 4, std::begin(conf.signature));
    conf.version = sound_config_version;
    conf.volumeSE = volumeSE_;
    conf.volumeBGM = volumeBGM_;

    FILE* fp = nullptr;
    fopen_s(&fp, sound_config_file_path, "wb");
    if (fp) {
        fwrite(&conf, sizeof(conf), 1, fp);
        fclose(fp);
    }
}

void SoundManager::set3DSoundInfo(VECTOR pos, float audioRange, const char* name)
{
    assert(nameAndHandleTable_[name] != -1);
    //サウンドのポジションを決める
    Set3DPositionSoundMem(pos, nameAndHandleTable_[name]);
    //サウンドが聞える範囲を設定
    Set3DRadiusSoundMem(audioRange, nameAndHandleTable_[name]);
}

void SoundManager::set3DSoundListenerInfo(VECTOR pos, VECTOR rot)
{
    //リスナーの位置と向きを設定
    Set3DSoundListenerPosAndFrontPos_UpVecY(pos, rot);
}
