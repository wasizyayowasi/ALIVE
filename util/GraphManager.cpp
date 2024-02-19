#include "GraphManager.h"

#include "DrawFunctions.h"

#include <DxLib.h>

namespace {
	//ファイルパス
	const char* const key_filepath				= "data/graph/key.png";
	const char* const title_filepath			= "data/graph/title.png";
	const char* const controller_filepath		= "data/graph/controller.png";
	const char* const controllerBottan_filepath = "data/graph/controllerBotton.png";
	const char* const chapter1_filepath			= "data/model/room/texture/Chapter1.png";
	const char* const chapter2_filepath			= "data/model/room/texture/Chapter2.png";
	const char* const chapter3_filepath			= "data/model/room/texture/Chapter3.png";
}

GraphManager::~GraphManager()
{
	for (auto& graph : graphHandle_) {
		DeleteGraph(graph.second);
	}
}

void GraphManager::Load()
{
	//画像をロードする
	graphHandle_["key"]					= Graph::LoadGraph(key_filepath);
	graphHandle_["title"]				= Graph::LoadGraph(title_filepath);
	graphHandle_["chapter1"]			= Graph::LoadGraph(chapter1_filepath);
	graphHandle_["chapter2"]			= Graph::LoadGraph(chapter2_filepath);
	graphHandle_["chapter3"]			= Graph::LoadGraph(chapter3_filepath);
	graphHandle_["controller"]			= Graph::LoadGraph(controller_filepath);
	graphHandle_["controllerBotton"]	= Graph::LoadGraph(controllerBottan_filepath);
}

int GraphManager::GetGraph(std::string name)
{
	return graphHandle_[name];
}
