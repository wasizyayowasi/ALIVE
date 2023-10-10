#include "EffekseerManager.h"
#include <EffekseerForDXLib.h>
#include <cassert>

EffekseerManager::EffekseerManager()
{
}

EffekseerManager::~EffekseerManager()
{
}

void EffekseerManager::Init()
{
    //loadEffekseerFile();

  // Effekseerを初期化する。
  //引数には画面に表示する最大パーティクル数を設定する。
    assert(Effkseer_Init(8000) != -1);
}

void EffekseerManager::End()
{
    for (auto& effect : nameAndHandleTable_) {
        DeleteEffekseerEffect(effect.second);
    }

    Effkseer_End();
}

void EffekseerManager::Update(const char* name)
{

    Effekseer_Sync3DSetting();

    if (time % 60 == 0) {

    }

    UpdateEffekseer3D();
}

void EffekseerManager::Draw(const char* name)
{
    for (int i = 0; i < nameAndHandleTable_.size(); i++) {
        DrawEffekseer3D_Begin();
        DrawEffekseer3D_Draw(nameAndHandleTable_[name]);
    }
}

int EffekseerManager::LoadEffekseerFile(const char* fileName)
{
    std::string path = "data/Effekseer/";
    path += fileName;
    path += ".efkefc";

    int handle = LoadEffekseerEffect(path.c_str());
    assert(handle != -1);

    nameAndHandleTable_[fileName] = handle;

    return handle;
}
