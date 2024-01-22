#include "LoadingScene.h"
#include "../util/ModelManager.h"

LoadingScene::LoadingScene(SceneManager& manager):SceneBase(manager)
{
}

LoadingScene::~LoadingScene()
{
}

void LoadingScene::Init()
{
	SetUseASyncLoadFlag(true);

	ModelManager::GetInstance().LoadModel();
}

void LoadingScene::End()
{
}

void LoadingScene::Update()
{
}

void LoadingScene::Draw()
{
	int num = GetASyncLoadNum();

	if (num < 1) {
		SetUseASyncLoadFlag(false);
	}

	printfDx("%d\n", num);

}
