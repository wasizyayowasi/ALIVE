#include "DxLib.h"
#include "EffekseerForDXLib.h"

#include "Scene/SceneManager.h"
#include "Scene/SceneTitle.h"
#include "Scene/GameMain.h"
#include "scene/DebugScene.h"

#include "util/game.h"
#include "util/InputState.h"
#include "util/ExternalFile.h"
#include "util/EffekseerManager.h"

#include "staging/Broom.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//window���[�h�̐ݒ�
	ChangeWindowMode(Game::window_mode);

	//��ʃ��[�h�ύX���O���t�B�b�N�V�X�e����O���t�B�b�N�n���h�������Z�b�g���邩
	//(true�F����Afalse�F���Ȃ�)
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	//window���ݒ�
	SetMainWindowText(Game::title_text);

	//��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::screen_width,Game::screen_height, Game::screen_depth);

	//3D�T�E���h��XAudio���g�p���邩
	SetEnableXAudioFlag(true);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	EffekseerManager::GetInstance().Init();
	
	//// �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ��B
	//Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	//�_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager manager;

//	manager.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager)));
	manager.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager)));

	while (ProcessMessage() == 0) {

		LONGLONG time = GetNowHiPerformanceCount();

		Effekseer_Sync3DSetting();

		//��ʂ̃N���A
		ClearDrawScreen();

		InputState::GetInstance().Update();

		manager.Update();
		manager.Draw();

		auto fps = GetFPS();
		auto DC = GetDrawCallCount();

		DrawFormatString(Game::screen_width - 200, 0, 0x448844, "FPS : %2.2f", fps);
		DrawFormatString(Game::screen_width - 200, 16, 0x448844, "DC : %d", DC);

		//����ʂ�\��ʂɐ؂�ւ���
		ScreenFlip();

		//esc����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))break;

		//fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667) {

		}

		if (manager.End()) {
			break;
		}

	}

	EffekseerManager::GetInstance().End();

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}