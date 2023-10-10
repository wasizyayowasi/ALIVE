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

  // Effekseer������������B
  //�����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
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
