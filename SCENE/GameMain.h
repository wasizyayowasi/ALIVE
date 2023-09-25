#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <memory>
#include <vector>

class Camera;
class Player;
class Broom;
class DepthOfField;
class Model;
class CharacterBase;
class GimmickBase;

class GameMain : public SceneBase
{
public:
	///�R���X�g���N�^
	GameMain(SceneManager& manager);
	//�f�X�g���N�^
	virtual ~GameMain();

	//�X�V
	void update(const InputState& input);
	//�`��
	void draw();

private:

	//������������Ȃ�
	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);

	//������������������Ȃ�
	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	std::shared_ptr<Camera> camera_;			//�J�����̃V�F�A�[�h�|�C���^
	std::shared_ptr<Player> player_;			//�v���C���[�̃V�F�A�[�h�|�C���^
	std::shared_ptr<Broom> broom_;				//�����������������Ȃ�
	std::shared_ptr<DepthOfField> depthOfField_;//�����������������Ȃ�
	std::shared_ptr<CharacterBase> enemy_;
	std::shared_ptr<GimmickBase> switch_;
	
	std::vector<std::shared_ptr<Model>> models_;//�Փ˔�����s���\��̃��f�����ЂƂ܂Ƃ߂ɂ���z��

	void (GameMain::* updateFunc_)(const InputState& input);		//�����o�֐��|�C���^

};

