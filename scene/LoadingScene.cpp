#include "LoadingScene.h"
#include "DebugScene.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "../util/game.h"
#include "../util/ModelManager.h"
#include "../util/FontsManager.h"
#include "../util/UIItemManager.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"

LoadingScene::LoadingScene(SceneManager& manager):SceneBase(manager)
{
}

LoadingScene::~LoadingScene()
{
}

void LoadingScene::Init()
{
	//�Z�k��
	auto& font = FontsManager::GetInstance();

	//�C���X�^���X��
	UIManager_ = std::make_shared<UIItemManager>();

	//UI�摜�쐬
	UIManager_->AddingMenuWithSplitStr(Game::screen_width - 230, Game::screen_height - 80, Game::screen_width, Game::screen_height, "Loading...", font.GetFontHandle("�s�O�� 0042"));

	//�񓯊������̊J�n
	SetUseASyncLoadFlag(true);

	//���f���̃��[�h
	ModelManager::GetInstance().LoadModel();

	//�O���t�@�C���̃��[�h
	ExternalFile::GetInstance().LoadFile();

	//�f�[�^�̃��[�h
	ExternalFile::GetInstance().LoadArrangementData();

	//�T�E���h�t�@�C���̃��[�h
	SoundManager::GetInstance().LoadSound();
}

void LoadingScene::End()
{
}

void LoadingScene::Update()
{
	//�񓯊��ǂݍ��ݒ��̐����擾
	aSyncLoadNum_ = GetASyncLoadNum();

	if (aSyncLoadNum_ < 1) {
		SetUseASyncLoadFlag(false);
		//manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager_)));
	}
}

void LoadingScene::Draw()
{
	UIManager_->ChangePosDraw(Game::screen_width - 230, Game::screen_height - 80);
}
