#include "KeyConfigScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "PopUpTextScene.h"
#include"../util/InputState.h"
#include"../util/game.h"
#include"../util/DrawFunctions.h"
#include "DxLib.h"
#include <algorithm>

//�R���X�g���N�^
KeyConfigScene::KeyConfigScene(SceneManager& manager, const InputState& input):SceneBase(manager),updateFunc_(&KeyConfigScene::selectChangeKeyUpdate), inputState_(input)
{
	keyTypeHandle_ = Graph::loadGraph("data/graph/key.png");
}

//�f�X�g���N�^
KeyConfigScene::~KeyConfigScene()
{
	//���݂̃L�[���͏����O���f�[�^�Ƃ��ď����o��
	//inputState_.savekeyInfo();
	inputState_.savekeyInfo2();
	DeleteGraph(keyTypeHandle_);
}

//�����o�֐��|�C���^�̍X�V
void KeyConfigScene::update(const InputState& input)
{
	(this->*updateFunc_)();
}

//�`��
void KeyConfigScene::draw()
{
	//�����폜����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawFormatString(50, 16 * selectNum_ + 32, color_, "��");

	int y = 32;
	for (auto& keyName : inputState_.inputNameTable_) {
		DrawFormatString(0, y, 0xffffff, "%s", keyName.second.c_str());
		y += 16;
	}

	DrawString(0, y, "�ύX", 0xffffff);
	y += 16;
	DrawString(0, y, "�L�����Z��", 0xffffff);

	
	//----------------�ȍ~�����\��
	DrawString(0, 0, "keyConfig",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "%d", selectNum_);
}

//�ύX�������L�[��I������
void KeyConfigScene::selectChangeKeyUpdate()
{
	//�Z�k��
	auto& configInput = const_cast<InputState&>(inputState_);
	//���݂̃L�[�̐����擾����
	const int keyNum = static_cast<int>(inputState_.inputNameTable_.size() + 2);

	//�I�𑀍�
	{
		if (inputState_.isPressed(InputType::up)) {
			selectNum_ = (std::max)(selectNum_ - 1, 0);
		}
		if (inputState_.isPressed(InputType::down)) {
			selectNum_ = (std::min)(selectNum_ + 1, keyNum - 1);
		}
	}

	//�L�[�̕ύX��ۑ�����
	if (selectNum_ == inputState_.inputNameTable_.size()) {
		if (inputState_.isTriggered(InputType::next)) {
			configInput.commitChangedInputInfo();		//���̃L�[�������ۂɎQ�Ƃ��Ă���L�[���ɑ������
			manager_.pushScene(std::shared_ptr<SceneBase>(std::make_shared<PopUpTextScene>(manager_)));		//�V�[�����|�b�v�A�b�v�e�L�X�g��`�悷��V�[���ɕύX����
			return;
		}
	}

	//���L�[�����������ă|�[�Y�V�[���ɖ߂�
	if (selectNum_ == inputState_.inputNameTable_.size() + 1) {
		if (inputState_.isTriggered(InputType::next)) {
			configInput.resetInputInfo();
			manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
			return;
		}
	}
	
	//�ǂ̃L�[��ύX���邩��������
	if (inputState_.isTriggered(InputType::next)) {
		updateFunc_ = &KeyConfigScene::changeKeyUpdate;
		return;
	}

	//�ЂƂO�̃V�[���ɖ߂�
	if (inputState_.isTriggered(InputType::prev)) {
		manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		return;
	}

}

//�ύX�������L�[���ǂ̃L�[�ɕύX����̂�
void KeyConfigScene::changeKeyUpdate()
{
	//�Z�k��
	auto& configInput = const_cast<InputState&>(inputState_);

	//�ύX�r���̏ꍇ���̐F��ς���
	if (inputState_.isTriggered(InputType::next)) {
		isEditing_ = !isEditing_;
		if (isEditing_) {
			color_ = 0x00ff00;
		}
		else {
			color_ = 0xff0000;
		}
	}

	//�L�[�{�[�h�ƃp�b�h�̓��͂𓾂�
	if (isEditing_) {
		char keyState[256];
		GetHitKeyStateAll(keyState);
		auto padState = GetJoypadInputState(DX_INPUT_PAD1);

		int idx = 0;
		InputType currentType = InputType::max;
		//���݂̑I��inputNameTable���擾����
		for (const auto& name : inputState_.inputNameTable_) {
			if (selectNum_ == idx) {
				currentType = name.first;
				break;
			}
			idx++;
		}

		//�L�[�{�[�h�̓��͂�ύX����
		for (int i = 0; i < 256; i++) {
			if (keyState[i]) {
				configInput.rewriteInputInfo(currentType, InputCategory::keybd, i);
				break;
			}
		}
		//�p�b�h�̓��͂�ύX����
		if (padState != 0) {
			configInput.rewriteInputInfo(currentType, InputCategory::pad, padState);
		}
	}
	
	//�����o�֐��|�C���^��ύX����L�[��I������֐��ɕύX����
	if (inputState_.isTriggered(InputType::prev)) {
		updateFunc_ = &KeyConfigScene::selectChangeKeyUpdate;
		isEditing_ = !isEditing_;
		color_ = 0xff0000;
		return;
	}

}

char KeyConfigScene::getKeyName(int num)
{
	switch (num) {
	
	}

	return 0;
}
