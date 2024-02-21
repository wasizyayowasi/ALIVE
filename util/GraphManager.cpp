#include "GraphManager.h"

#include "DrawFunctions.h"

#include "../util/ExternalFile.h"

#include <DxLib.h>

//�f�X�g���N�^
GraphManager::~GraphManager()
{
	for (auto& graph : graphHandle_) {
		DeleteGraph(graph.second);
	}
}

//�摜�̓ǂݍ���
void GraphManager::Load()
{
	//�摜�����[�h����
	for (auto& place : ExternalFile::GetInstance().GetGraphFilePath())
	{
		for (auto& name : place.second)
		{
			std::string path = "data/" + place.first + "/" + name + ".png";
			graphHandle_[name] = Graph::LoadGraph(path.c_str());
		}
	}
}

//�摜�̎擾
int GraphManager::GetGraph(std::string name)
{
	return graphHandle_[name];
}
