#include "EffekseerManager.h"
#include <EffekseerForDXLib.h>
#include <cassert>

EffekseerManager::EffekseerManager()
{
    //loadEffekseerFile();
}

EffekseerManager::~EffekseerManager()
{
    for (auto& effect : nameAndHandleTable_) {
        DeleteEffekseerEffect(effect.second);
    }
}

void EffekseerManager::update(const char* name)
{

    Effekseer_Sync3DSetting();

    if (time % 60 == 0) {

    }

    UpdateEffekseer3D();
}

void EffekseerManager::draw(const char* name)
{
    for (int i = 0; i < nameAndHandleTable_.size(); i++) {
        DrawEffekseer3D_Begin();
        DrawEffekseer3D_Draw(nameAndHandleTable_[name]);
    }
}

int EffekseerManager::loadEffekseerFile(const char* fileName)
{
    std::string path = "data/Effekseer/";
    path += fileName;
    path += ".efkefc";

    int handle = LoadEffekseerEffect(path.c_str());
    assert(handle != -1);

    nameAndHandleTable_[fileName] = handle;

    return handle;
}
