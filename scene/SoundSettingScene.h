#pragma once
#include "SceneBase.h"
#include <memory>
#include <string>

class InputState;
class UIItemManager;

class SoundSettingScene : public SceneBase
{
public:

	SoundSettingScene(SceneManager& manager);
	virtual ~SoundSettingScene();

	void Init();
	void End();

	void Update(const InputState& input);
	void Draw();

	void NormalUpdate(const InputState& input);
	void GaussFadeInUpdate(const InputState& input);
	void GaussFadeOutUpdate(const InputState& input);

	void BGMUpdate(const InputState& input);
	void SEUpdate(const InputState& input);
	void ChangeWindowUpdate(const InputState& input);

	void MovePictogram(int pictPos, float& pos, float& rot,bool& inversion);

	void ChangeUpdateFunc(const InputState& input);

private:

	int makeScreenHandle_ = -1;

	int BGMBarHandle_ = -1;
	int SEBarHandle_ = -1;
	int afterProcessingBGMBarGraph_ = -1;
	int afterProcessingSEBarGraph_ = -1;
	int pictogramGraph_ = -1;

	int volumeBGM_ = 0;
	int volumeSE_ = 0;

	int barHandleWidth_ = 0;
	int barHandleHeight_ = 0;

	int selectNum_ = 0;
	int time_ = 0;

	float BGMPictogramPosX_ = 0.0f;				//BGMピクトグラムのポジション
	float SEPictogramPosX_ = 0.0f;				//SEピクトグラムのポジション

	float BGMPictogramRot_ = 0.1f;				//BGMピクトグラムの傾き
	float SEPictogramRot_ = 0.1f;				//SEピクトグラムの傾き

	bool BGMPictogramTransInversion_ = false;	//BGMピクトグラム画像のの反転
	bool SEPictogramTransInversion_ = false;	//SEピクトグラム画像のの反転


	//ここも消すかもしれない
	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	std::shared_ptr<SceneBase> nextScene_;

	std::string windowModeText_ = "≪  ウィンドモード  ≫";

	std::shared_ptr<UIItemManager> UIManager_;

	void(SoundSettingScene::* updateFunc_)(const InputState& input);

};

