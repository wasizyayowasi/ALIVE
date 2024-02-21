#include "GraphManager.h"

#include "DrawFunctions.h"

#include "../util/ExternalFile.h"

#include <DxLib.h>

//デストラクタ
GraphManager::~GraphManager()
{
	for (auto& graph : graphHandle_) {
		DeleteGraph(graph.second);
	}
}

//画像の読み込み
void GraphManager::Load()
{
	//画像をロードする
	for (auto& place : ExternalFile::GetInstance().GetGraphFilePath())
	{
		for (auto& name : place.second)
		{
			std::string path = "data/" + place.first + "/" + name + ".png";
			graphHandle_[name] = Graph::LoadGraph(path.c_str());
		}
	}
}

//画像の取得
int GraphManager::GetGraph(std::string name)
{
	return graphHandle_[name];
}
