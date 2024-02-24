#include "ModelManager.h"

#include "../util/ExternalFile.h"

//�f�X�g���N�^
ModelManager::~ModelManager()
{
	for (auto& type : modelHandle_)
	{
		MV1DeleteModel(type.second);
	}
}

//���f���̃��[�h
void ModelManager::LoadModel()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//���f���̃��[�h
	for (auto& type : file.GetModelFilePath())
	{
		for (auto& name : type.second)
		{
			std::string path = "data/model/" + type.first + "/mv1/" + name + ".mv1";
			modelHandle_[name] = MV1LoadModel(path.c_str());
		}
	}
}

//3D���f���n���h�����擾����
int ModelManager::GetModelHandle(const std::string& name)
{
	return modelHandle_[name];
}
