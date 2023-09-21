#include "DxLib.h"

#include "Scene/SceneManager.h"
#include "Scene/SceneTitle.h"
#include "Scene/GameMain.h"
#include "scene/DebugScene.h"

#include "util/game.h"
#include "util/InputState.h"
#include "util/LoadExternalFile.h"

#include "staging/Broom.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//windowモードの設定
	ChangeWindowMode(Game::kWindowMode);

	//window名設定
	SetMainWindowText(Game::kTitleText);

	//画面サイズの設定
	SetGraphMode(Game::kScreenWidth,Game::kScreenHeight, Game::kScreenDepth);

	//3DサウンドにXAudioを使用するか
	SetEnableXAudioFlag(true);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// シェーダーモデル２．０が使用できるかどうかをチェック
	if (GetValidShaderVersion() < 200)
	{
		DrawString(0, 0, "シェーダーモデル２．０が使用できません", GetColor(255, 255, 255));
		DxLib_End();
		return 0;
	}

	//ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	InputState input;
	SceneManager manager;
	
	//manager.changeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager)));
	manager.changeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager)));

	while (ProcessMessage() == 0) {

		LONGLONG time = GetNowHiPerformanceCount();

		//画面のクリア
		ClearDrawScreen();

		input.update();

		manager.update(input);
		manager.draw();

		auto fps = GetFPS();
		auto DC = GetDrawCallCount();

		LoadExternalFile::getInstance().draw();

		DrawFormatString(1180, 0, 0xffff00, "FPS : %2.2f", fps);
		DrawFormatString(1200, 16, 0xffff00, "DC : %d", DC);

		//裏画面を表画面に切り替える
		ScreenFlip();

		//escを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))break;

		//fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667) {

		}

	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}