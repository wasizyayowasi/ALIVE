#include "DxLib.h"
#include "util/game.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneTitle.h"
#include "Scene/GameMain.h"
#include "util/InputState.h"
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

	//Direct3D9Exを使用する
	SetUseDirect3DVersion(DX_DIRECT3D_9EX);

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
	manager.changeScene(new GameMain(manager));

	while (ProcessMessage() == 0) {

		LONGLONG time = GetNowHiPerformanceCount();

		//画面のクリア
		ClearDrawScreen();

		input.update();

		manager.update(input);
		manager.draw();

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