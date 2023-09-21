#include "DxLib.h"

#include "Scene/SceneManager.h"
#include "Scene/SceneTitle.h"
#include "Scene/GameMain.h"
#include "scene/DebugScene.h"

#include "util/game.h"
#include "util/InputState.h"
#include "util/LoadExternalFile.h"

#include "staging/Broom.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//window���[�h�̐ݒ�
	ChangeWindowMode(Game::kWindowMode);

	//window���ݒ�
	SetMainWindowText(Game::kTitleText);

	//��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::kScreenWidth,Game::kScreenHeight, Game::kScreenDepth);

	//3D�T�E���h��XAudio���g�p���邩
	SetEnableXAudioFlag(true);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// �V�F�[�_�[���f���Q�D�O���g�p�ł��邩�ǂ������`�F�b�N
	if (GetValidShaderVersion() < 200)
	{
		DrawString(0, 0, "�V�F�[�_�[���f���Q�D�O���g�p�ł��܂���", GetColor(255, 255, 255));
		DxLib_End();
		return 0;
	}

	//�_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	InputState input;
	SceneManager manager;
	
	//manager.changeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager)));
	manager.changeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager)));

	while (ProcessMessage() == 0) {

		LONGLONG time = GetNowHiPerformanceCount();

		//��ʂ̃N���A
		ClearDrawScreen();

		input.update();

		manager.update(input);
		manager.draw();

		auto fps = GetFPS();
		auto DC = GetDrawCallCount();

		LoadExternalFile::getInstance().draw();

		DrawFormatString(1180, 0, 0xffff00, "FPS : %2.2f", fps);
		DrawFormatString(1200, 16, 0xffff00, "DC : %d", DC);

		//����ʂ�\��ʂɐ؂�ւ���
		ScreenFlip();

		//esc����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))break;

		//fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667) {

		}

	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}