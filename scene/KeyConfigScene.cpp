#include "KeyConfigScene.h"
#include "SceneManager.h"
#include "SettingScene.h"
#include "PopUpTextScene.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"
#include "../util/GraphManager.h"
#include "../util/DrawFunctions.h"
#include "../util/UIItemManager.h"

#include <DxLib.h>
#include <algorithm>
#include <string>

namespace {
	//key�摜�̈ꖇ������̃T�C�Y
	constexpr float graph_chip_size = 59.0f;
	//�摜�̌��ԃT�C�Y
	constexpr float graph_gap_size = 10.0f;
}

//�R���X�g���N�^
KeyConfigScene::KeyConfigScene(SceneManager& manager):SceneBase(manager),
updateFunc_(&KeyConfigScene::FadeInUpdate)
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�Ō�ɓ��͂��ꂽ���u�ɂ���čX�V�A�`��֐���ύX����
	if (!input.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigScene::ControllerUpdate;
		drawFunc_ = &KeyConfigScene::ControllerDraw;
	}
	else {
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
	}
}

//�f�X�g���N�^
KeyConfigScene::~KeyConfigScene()
{
}

//������
void KeyConfigScene::Init()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�C���X�^���X��
	KeyUI_ = std::make_shared<UIItemManager>();
	PadUI_ = std::make_shared<UIItemManager>();

	//�X�N���[���T�C�Y�̃n���h�����쐬
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	//�t�H���g�̍쐬
	fontHandleSize21_ = FontsManager::GetInstance().GetFontHandle("�s�O�� 0021");
	fontHandleSize42_ = FontsManager::GetInstance().GetFontHandle("�s�O�� 0042");
	
	//�L�[�̖����̈�ڂ̍��W
	float namePosX = static_cast<float>(Game::screen_width / 4);
	float namePosY = static_cast<float>(Game::screen_height / 2) - (graph_chip_size * 4 + 45.0f);

	int nameNo = 0;
	for (auto& table : input.inputNameTable_) {
		//���j���[�̒ǉ�
		KeyUI_->AddMenu(namePosX, namePosY, 320, 100, table.second.c_str(), fontHandleSize21_);

		//�|�W�V��������
		namePosY += graph_chip_size + graph_gap_size;

		//inputstate.tempmaptable�̔����𒴂�����܂�Ԃ�
		if (nameNo == input.tempMapTable_.size() / 2 - 1) {
			namePosY = static_cast<float>(static_cast<float>(Game::screen_height / 2) - (graph_chip_size * 4 + 30.0f));
			namePosX += static_cast<float>(Game::screen_width / 4) * 1.5f;
		}

		nameNo++;
	}

	//���j���[�̒ǉ�
	KeyUI_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 5 * 4), 320, 100, "�ύX", fontHandleSize42_);
	KeyUI_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 5 * 4 + 32), 320, 100, "�L�����Z��", fontHandleSize42_);
	PadUI_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 5 * 4 + 32), 320, 100, "�L�����Z��", fontHandleSize42_);
}

//�I��
void KeyConfigScene::End()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//���݂̃L�[���͏����O���f�[�^�Ƃ��ď����o��
	input.SavekeyInfo();
	DeleteGraph(makeScreenHandle_);
}

//�X�V
void KeyConfigScene::Update()
{
	(this->*updateFunc_)();
}

//�`��
void KeyConfigScene::Draw()
{
	//�w�i�ɍ��̓���
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 8, 255 - fadeValue_);

	//makeScreen�ō쐬�����n���h����`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawGraph(0, 0, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	(this->*drawFunc_)();
}

//�L�[�̖�����̕`��
void KeyConfigScene::KeyStateDraw()
{
	//�������݃X�N���[���̕ύX
	SetDrawScreen(makeScreenHandle_);

	//�X�N���[���̃N���A
	ClearDrawScreen();

	//�L�[�̖�����`�悷��
	KeyUI_->AlphaChangeDraw(selectNum_,fadeValue_);

	//�L�[�ɑΉ�����摜��`�悷��
	KeyGraphDraw();

	//�������݃X�N���[���𗠂̃X�N���[���ɕύX����
	SetDrawScreen(DX_SCREEN_BACK);
}

//�L�[�̉摜��`�悷��
void KeyConfigScene::KeyGraphDraw()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�|�W�V�����ݒ�
	float graphPosX = static_cast<float>(Game::screen_width) / 4 * 1.5f;
	float graphPosY = static_cast<float>(Game::screen_height / 2) - (graph_chip_size * 4 + 30.0f);

	//for���ŉ��Ԗڂ����擾����
	int keyCount = 0;

	//���邳�������l
	int subBrightness = 180;

	//�摜�̊g�k��
	float graphScale = 1.0f;

	for (auto& key : input.tempMapTable_) {
		
		if (key.first == InputType::Creative) {
			continue;
		}

		//���݃J�[�\���������Ă���ꍇ
		//�T�C�Y�Ɩ��邳�������l��ύX����
		if (keyCount == selectNum_) {
			graphScale = 1.2f;
			subBrightness = 0;
		}
		else {
			graphScale = 1.0f;
			subBrightness = 180;
		}

		//keyType�̕`��
		input.DrawKeyGraph(static_cast<int>(key.first), graphPosX, graphPosY, graphScale);

		//�Â������摜���摜�̏�ɏ悹��
		SetDrawBlendMode(DX_BLENDMODE_SUB, subBrightness);
		DrawBoxAA(graphPosX - 80.0f, graphPosY - graph_chip_size / 2 - graph_gap_size / 2, graphPosX - 240.0f + static_cast<float>(Game::screen_width / 4), graphPosY + graph_chip_size / 2 + graph_gap_size / 2, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//�����𒴂�����܂�Ԃ�����
		graphPosY += graph_chip_size + 10;
		if (keyCount == input.tempMapTable_.size() / 2 - 1) {
			graphPosY = static_cast<float>(Game::screen_height / 2) - (graph_chip_size * 4 + 30.0f);
			graphPosX += static_cast<float>(Game::screen_width / 4) * 1.5f;
		}

		keyCount++;
	}
}

//�R���g���[���[�摜�̕`��
void KeyConfigScene::ControllerDraw()
{
	//�������݃X�N���[���̕ύX
	SetDrawScreen(makeScreenHandle_);

	//�X�N���[���̃N���A
	ClearDrawScreen();

	//UI�̕`��
	PadUI_->AlphaChangeDraw(selectNum_, fadeValue_);

	//��������摜�̕`��
	DrawGraph(0, 0, GraphManager::GetInstance().GetGraph("controller"), true);

	//�������݃X�N���[���𗠂̃X�N���[���ɕύX����
	SetDrawScreen(DX_SCREEN_BACK);
}

//�ύX������key����͂����邽�߂̃|�b�v�A�b�v�`��
void KeyConfigScene::ChangeKeyPopUpText()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�w�i�̍��`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(Game::screen_width / 6, Game::screen_height / 5, Game::screen_width / 6 * 5, Game::screen_height / 5 * 4, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	//������
	std::string text = input.inputNameTable_.find(static_cast<InputType>(selectNum_))->second;

	//������̉���
	int strWidth = GetDrawStringWidthToHandle(text.c_str(), static_cast<int>(strlen(text.c_str())), fontHandleSize21_);

	//�I�������L�[�̖��O���o��
	DrawStringToHandle(Game::screen_width / 6 + strWidth / 2 , Game::screen_height / 5 - static_cast<int>(graph_chip_size / 2 - 8), text.c_str(), 0xffffff, fontHandleSize21_);

	//int�^�ɃL���X�g����graphChipSize
	int castGraphChipSize = static_cast<int>(graph_chip_size);

	//�L�[�摜�̕`��
	input.DrawKeyGraph( selectNum_, static_cast<float>(Game::screen_width / 6) + strWidth * 2.5f,
						static_cast<float>(Game::screen_height / 5) - graph_chip_size / 2,
						1.0f);

	//������
	text = "�ύX�������L�[����͂��Ă�������";
	//������̉���
	strWidth = GetDrawStringWidthToHandle(text.c_str(), static_cast<int>(strlen(text.c_str())), fontHandleSize42_);
	//������̕`��
	DrawStringToHandle(Game::screen_width / 2 - strWidth / 2, Game::screen_height / 2, text.c_str(), 0xffffff, fontHandleSize42_);

}

//�ǂ̓��͑��u�̃L�[��ύX���邩��I������
void KeyConfigScene::SelectChangeKeyUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//���݂̃L�[�̐����擾����
	const int keyNum = static_cast<int>(input.inputNameTable_.size() + 2);

	//�I�𑀍�
	if (input.IsTriggered(InputType::Up)) {
		if (selectNum_ == (keyNum - 1) / 2) {
			selectNum_ += keyNum / 2;
		}
		else {
			selectNum_ = ((selectNum_ - 1) + keyNum) % keyNum;
		}
	}
	else if (input.IsTriggered(InputType::Down)) {
		if (selectNum_ + 1 == (keyNum - 1) / 2) {
			selectNum_ += keyNum / 2;
		}
		else {
			selectNum_ = (selectNum_ + 1) % keyNum;
		}
	}
	else if (input.IsTriggered(InputType::Left) || input.IsTriggered(InputType::Right)) {
		if (selectNum_ < keyNum - 2) {
			selectNum_ = (selectNum_ + 7) % (keyNum - 1);
		}
	}

	//�L�[�̕ύX��ۑ�����
	if (selectNum_ == input.inputNameTable_.size()) {
		if (input.IsTriggered(InputType::Space)) {

			//���̃L�[�������ۂɎQ�Ƃ��Ă���L�[���ɑ������
			input.CommitChangedInputInfo();

			//�V�[�����|�b�v�A�b�v�e�L�X�g��`�悷��V�[���ɕύX����
			manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<PopUpTextScene>(manager_)));		
			return;
		}
	}

	//���L�[�����������ă|�[�Y�V�[���ɖ߂�
	if (selectNum_ == input.inputNameTable_.size() + 1) {
		if (input.IsTriggered(InputType::Space)) {
			input.ResetInputInfo();
			updateFunc_ = &KeyConfigScene::FadeOutUpdate;
			return;
		}
	}
	
	//�ǂ̃L�[��ύX���邩��������
	if (input.IsTriggered(InputType::Space)) {
		isEditing_ = !isEditing_;
		drawFunc_ = &KeyConfigScene::ChangeKeyPopUpText;
		changeKeyUpdateFunc_ = &KeyConfigScene::ChangeKeyborardUpdate;
		GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 32, 800);
		return;
	}

	//�ЂƂO�̃V�[���ɖ߂�
	if (input.IsTriggered(InputType::Pause)) {
		updateFunc_ = &KeyConfigScene::FadeOutUpdate;
		input.RollbackChangedInputInfo();
		return;
	}

	//�R���g���[���[�̓��͂��������Ƃ���
	//�R���g���[���[�p�̕`��ƍX�V�p�N���X�ɕύX����
	if (!input.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigScene::ControllerUpdate;
		drawFunc_ = &KeyConfigScene::ControllerDraw;
	}

}

//�ύX����L�[���ǂ̃L�[�ɕύX����̂������肷��
void KeyConfigScene::ChangeKeyborardUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�L�[�{�[�h�ƃp�b�h�̓��͂𓾂�
	char keyState[256];
	GetHitKeyStateAll(keyState);

	int idx = 0;
	InputType currentType = InputType::Max;

	//��t���[���O�̓��͏�񂪎c���Ă����烊�^�[������
	for (const auto& key : input.lastInput_) {
		if (key == true) return;
	}

	//�����o�֐��|�C���^��ύX����L�[��I������֐��ɕύX����
	if (input.IsTriggered(InputType::Pause)) {
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		isEditing_ = !isEditing_;
		input.UndoSelectKey(static_cast<InputType>(selectNum_), InputCategory::keybd);
		return;
	}

	//���݂̑I��inputNameTable���擾����
	for (const auto& name : input.inputNameTable_) {
		if (selectNum_ == idx) {
			currentType = name.first;
			break;
		}
		idx++;
	}

	//�L�[�{�[�h�̓��͂�ύX����
	for (int i = 0; i < 256; i++) {
		if (keyState[i]) {
			drawFunc_ = &KeyConfigScene::KeyStateDraw;
			changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
			isEditing_ = !isEditing_;
			input.RewriteInputInfo(currentType, InputCategory::keybd, i);
			break;
		}
	}
}

//�R���g���[���[�̏ꍇ�̍X�V
void KeyConfigScene::ControllerUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//���L�[�����������ă|�[�Y�V�[���ɖ߂�
	if (selectNum_ == 0) {
		if (input.IsTriggered(InputType::Space)) {
			input.ResetInputInfo();
			updateFunc_ = &KeyConfigScene::FadeOutUpdate;
			return;
		}
	}

	//�ЂƂO�̃V�[���ɖ߂�
	if (input.IsTriggered(InputType::Pause)) {
		updateFunc_ = &KeyConfigScene::FadeOutUpdate;
		input.RollbackChangedInputInfo();
		return;
	}

	//�L�[�{�[�h�̓��͂��������Ƃ���
	//�L�[�{�[�h�p�̕`��ƍX�V�p�N���X�ɕύX����
	if (input.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
	}

}

//�t�F�[�h�C���̍X�V
void KeyConfigScene::FadeInUpdate()
{
	float timer = static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_);
	fadeValue_ = static_cast <int>(255 * timer);
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &KeyConfigScene::NormalUpdate;
		fadeValue_ = 255;
	}
}

//�ʏ펞�̍X�V
void KeyConfigScene::NormalUpdate()
{
	(this->*changeKeyUpdateFunc_)();
}

//�t�F�[�h�A�E�g�̍X�V
void KeyConfigScene::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<SettingScene>(manager_)));
		fadeValue_ = 0;
		return;
	}
}
