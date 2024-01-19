#include "SoundManager.h"
#include <cassert>

namespace {
    constexpr char sound_file_signature[] = "SND_";
    constexpr char sound_config_file_path[] = "./sound.conf";
    constexpr float sound_config_version = 1.0f;
}

using namespace std;

SoundManager::SoundManager() {
    //loadSoundConfig();
    Load2DSoundSEFile("alarm");
    Load2DSoundSEFile("stopAlarm");
    Load3DSoundBGMFile("cafe");
    Load3DSoundSEFile("openTheDoor");
    Load3DSoundSEFile("pullLever");
    Load3DSoundSEFile("switchOn");
    SetSEVolume(volumeSE_);
    SetBGMVolume(volumeBGM_);
}

int SoundManager::Load2DSoundSEFile(const char* fileName)
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

/// <summary>
/// 2Dサウンドをロードする
/// </summary>
/// <param name="fileName">拡張子、場所抜きのファイル単体の名前</param>
/// <returns>ロードしたサウンド</returns>
int SoundManager::Load2DSoundBGMFile(const char* fileName)
{
    //ファイルパスの生成
    string path = "data/sound/BGM/";
    path += fileName;
    path += ".mp3";

    //メモリに2Dサウンドをロードする
    int handle = LoadSoundMem(path.c_str());
    assert(handle >= 0);

    //配列にロードしたものを加える
    nameAndHandleTable_[fileName] = handle;

    return handle;
}

int SoundManager::Load3DSoundSEFile(const char* fileName)
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

int SoundManager::Load3DSoundBGMFile(const char* fileName)
{
    //ファイルパスの生成
    string path = "data/sound/BGM/";
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

void SoundManager::LoadSoundConfig()
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

void SoundManager::Play(const char* name)
{
    assert(nameAndHandleTable_[name] != -1);
    PlaySoundMem(nameAndHandleTable_[name], DX_PLAYTYPE_BACK);
}

void SoundManager::PlayBGM(const char* path)
{
    PlayMusic(path, DX_PLAYTYPE_LOOP);
    SetVolumeMusic(volumeBGM_);
}

void SoundManager::SetSEVolume(unsigned int volume)
{
    for (auto& record : nameAndHandleTable_) {
        ChangeVolumeSoundMem(volume, record.second);
    }
    volumeSE_ = volume;
}

int SoundManager::GetSEVolume() const
{
    return volumeSE_;
}

void SoundManager::SetBGMVolume(unsigned int volume)
{
    SetVolumeMusic(volume);
    volumeBGM_ = volume;
}

void SoundManager::SetBGMRate(float rate)
{
    assert(0.0f <= rate && rate <= 1.0f);
    SetVolumeMusic(static_cast<int>(static_cast<float>(volumeBGM_ * rate)));
}

int SoundManager::GetBGMVolume() const
{
    return volumeBGM_;
}

void SoundManager::StopSE(const char* name)
{
    if (name != nullptr) {
        StopSoundMem(nameAndHandleTable_[name]);
    }
}

void SoundManager::StopBGM()
{
    StopMusic();
}

void SoundManager::SaveSoundConfig()
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

void SoundManager::Set3DSoundInfo(VECTOR pos, float audioRange, const char* name)
{
    assert(nameAndHandleTable_[name] != -1);
    //サウンドのポジションを決める
    Set3DPositionSoundMem(pos, nameAndHandleTable_[name]);
    //サウンドが聞える範囲を設定
    Set3DRadiusSoundMem(audioRange, nameAndHandleTable_[name]);
}

void SoundManager::Set3DSoundListenerInfo(VECTOR pos, VECTOR rot)
{
    //リスナーの位置と向きを設定
    Set3DSoundListenerPosAndFrontPos_UpVecY(pos, rot);
}

int SoundManager::CheckSoundFile(const char* const name)
{
    return DxLib::CheckSoundMem(nameAndHandleTable_[name]);
}
