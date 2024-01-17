#pragma once
#include "SceneBase.h"
#include <memory>
#include <string>

class InputState;
class UIItemManager;

class SettingScene : public SceneBase
{
public:

	SettingScene(SceneManager& manager);
	virtual ~SettingScene();

	void Init();
	void End();

	void Update();
	void Draw();

	void NormalUpdate();
	void GaussFadeInUpdate();
	void GaussFadeOutUpdate();

	void BGMUpdate();
	void SEUpdate();
	void ChangeWindowUpdate();

	void MovePictogram(int pictPos, float& pos, float& rot,bool& inversion);

	void ChangeUpdateFunc();

private:

	int makeScreenHandle_ = -1;

	int circleTransWhiteHandle_ = -1;
	int circleWhiteHandle_ = -1;
	int pictogramHandle_ = -1;
	int houseHandle_ = -1;

	int volumeBGM_ = 0;
	int volumeSE_ = 0;

	int selectNum_ = 0;
	int time_ = 0;

	float BGMPictogramPosX_ = 0.0f;				//BGMピクトグラムのポジション
	float SEPictogramPosX_ = 0.0f;				//SEピクトグラムのポジション

	float BGMPictogramRot_ = 0.1f;				//BGMピクトグラムの傾き
	float SEPictogramRot_ = 0.1f;				//SEピクトグラムの傾き

	bool BGMPictogramTransInversion_ = false;	//BGMピクトグラム画像のの反転
	bool SEPictogramTransInversion_ = false;	//SEピクトグラム画像のの反転
	bool windowMode_ = true;					//ウィンドウモード

	//ここも消すかもしれない
	static constexpr int fadeInterval_ = 30;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	std::shared_ptr<SceneBase> nextScene_;

	std::string windowModeText_ = "≪  ウィンドウモード  ≫";

	std::shared_ptr<UIItemManager> UIManager_;

	void(SettingScene::* updateFunc_)();

};

