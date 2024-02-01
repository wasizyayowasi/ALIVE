#include "SoundManager.h"
#include <cassert>

namespace {
    constexpr char sound_file_signature[] = "SND_";
    constexpr char sound_config_file_path[] = "./sound.conf";
    constexpr float sound_config_version = 1.0f;
}

using namespace std;

SoundManager::SoundManager() 
{
    //3DサウンドにXAudioを使用するか
    SetEnableXAudioFlag(true);

    //3Dサウンドの1メートルをどのくらいの値にするか
    Set3DSoundOneMetre(1.0f);
}

SoundManager::~SoundManager()
{
}

void SoundManager::LoadSound()
{
    //LoadSoundConfig();
    Load2DSoundSEFile("alarm");
    Load2DSoundSEFile("stopAlarm");
    Load2DSoundSEFile("crank");
    Load2DSoundSEFile("door");
    Load2DSoundSEFile("checkSoundSE");
    Load2DSoundBGMFile("checkSoundBGM");

    Load3DSoundSEFile("hit");
    Load2DSoundSEFile("ironStep");
    Load3DSoundSEFile("pullLever");
    Load3DSoundSEFile("switchOn");
    Load2DSoundSEFile("asphaltStep");
    Load3DSoundBGMFile("cafe");
}

void SoundManager::ChangeVolumeMem()
{
    SetSEVolume(volumeSE_);
    SetBGMVolume(volumeBGM_);
}

int SoundManager::Load2DSoundSEFile(std::string fileName)
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

int SoundManager::Load2DSoundBGMFile(std::string fileName)
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

int SoundManager::Load3DSoundSEFile(std::string fileName)
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

int SoundManager::Load3DSoundBGMFile(std::string fileName)
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

void SoundManager::PlaySE(std::string name)
{
    assert(nameAndHandleTable_[name] != -1);

    //メモリに読み込んだサウンドを再生する
    PlaySoundMem(nameAndHandleTable_[name], DX_PLAYTYPE_BACK);
}

void SoundManager::PlayBGM(std::string name)
{
    //ファイルパスの生成
    std::string path = "data/sound/BGM/";
    path = path + name;
    path = path + ".mp3";

    PlayMusic(path.c_str(), DX_PLAYTYPE_BACK);
}

void SoundManager::SetSEVolume(unsigned int volume)
{
    for (auto& record : nameAndHandleTable_) {
        ChangeVolumeSoundMem(volume, record.second);
    }
    volumeSE_ = volume;
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

void SoundManager::StopSE(std::string name)
{
    assert(static_cast<int>(name.size()) > 0);

    //再生中のサウンドを止める
    StopSoundMem(nameAndHandleTable_[name]);
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

void SoundManager::Set3DSoundInfo(VECTOR pos, float audioRange, std::string name)
{
    assert(nameAndHandleTable_[name] != -1);

    //サウンドのポジションを決める
    Set3DPositionSoundMem(pos, nameAndHandleTable_[name]);

    //サウンドが聞こえる範囲を設定
    Set3DRadiusSoundMem(audioRange, nameAndHandleTable_[name]);
}

void SoundManager::Set3DSoundListenerInfo(VECTOR pos, VECTOR rot)
{
    //リスナーの位置と向きを設定
    Set3DSoundListenerPosAndFrontPos_UpVecY(pos, rot);
}

int SoundManager::CheckSoundFile(std::string name)
{
    return DxLib::CheckSoundMem(nameAndHandleTable_[name]);
}
