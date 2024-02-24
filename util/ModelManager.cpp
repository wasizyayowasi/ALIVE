#include "ModelManager.h"

#include "../util/ExternalFile.h"

//デストラクタ
ModelManager::~ModelManager()
{
	for (auto& type : modelHandle_)
	{
		MV1DeleteModel(type.second);
	}
}

//モデルのロード
void ModelManager::LoadModel()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//モデルのロード
	for (auto& type : file.GetModelFilePath())
	{
		for (auto& name : type.second)
		{
			std::string path = "data/model/" + type.first + "/mv1/" + name + ".mv1";
			modelHandle_[name] = MV1LoadModel(path.c_str());
		}
	}
}

//3Dモデルハンドルを取得する
int ModelManager::GetModelHandle(const std::string& name)
{
	return modelHandle_[name];
}
