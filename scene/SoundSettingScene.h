#pragma once
#include "SceneBase.h"
#include <memory>

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

	void BGMUpdate(const InputState& input);
	void SEUpdate(const InputState& input);
	void ChangeWindowUpdate(const InputState& input);

	void MovePictogram(int pictPos, float& pos, float& rot,bool& inversion);

	void ChangeUpdateFunc();

private:

	int BGMBarHandle_ = -1;
	int SEBarHandle_ = -1;
	int afterProcessingBarGraph_ = -1;
	int pictogramGraph_ = -1;

	int volumeBGM_ = 0;
	int volumeSE_ = 0;

	int barHandleWidth_ = 0;
	int barHandleHeight_ = 0;

	int selectNum_ = 0;
	int time_ = 0;

	float BGMPictogramPosX_ = 0.0f;				//BGM�s�N�g�O�����̃|�W�V����
	float SEPictogramPosX_ = 0.0f;				//SE�s�N�g�O�����̃|�W�V����

	float BGMPictogramRot_ = 0.1f;				//BGM�s�N�g�O�����̌X��
	float SEPictogramRot_ = 0.1f;				//SE�s�N�g�O�����̌X��

	bool BGMPictogramTransInversion_ = false;	//BGM�s�N�g�O�����摜�̂̔��]
	bool SEPictogramTransInversion_ = false;	//SE�s�N�g�O�����摜�̂̔��]

	std::shared_ptr<UIItemManager> UIManager_;

	void(SoundSettingScene::* updateFunc_)(const InputState& input);

};

