#include "LoadingScene.h"
#include "DebugScene.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "../util/game.h"
#include "../util/ModelManager.h"
#include "../util/FontsManager.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/EffectManager.h"
#include "../util/UIItemManager.h"

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

	//�O���t�@�C���̃��[�h
	ExternalFile::GetInstance().LoadFile();
	
	//���f���̃��[�h
	ModelManager::GetInstance().LoadModel();
	
	//�T�E���h�t�@�C���̃��[�h
	SoundManager::GetInstance().LoadSound();
	
	//�摜�̃��[�h
	EffectManager::GetInstance().Load();

	//�񓯊��������I������
	SetUseASyncLoadFlag(false);

	loadingFile_ = true;
}

void LoadingScene::End()
{
}

void LoadingScene::Update()
{
	//�񓯊��ǂݍ��ݒ��̐����擾
	aSyncLoadNum_ = GetASyncLoadNum();

	if (loadingFile_) {
		if (aSyncLoadNum_ < 1) {
			loadingFile_ = false;
			
			//�f�[�^�̃��[�h
			ExternalFile::GetInstance().LoadArrangementData();
		}
	}
	else {
		if (aSyncLoadNum_ < 1) {
			//manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager_)));
		}
	}
}

void LoadingScene::Draw()
{
	UIManager_->ChangePosDraw(Game::screen_width - 230, Game::screen_height - 80);
	//DrawFormatString(0, 0, 0xffffff, "%d", aSyncLoadNum_);
}
