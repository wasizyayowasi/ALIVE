#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <memory>
#include <list>
#include <unordered_map>

class Camera;
class Player;
class Broom;
class DepthOfField;
class Model;
class CharacterBase;
class CarryObjectBase;
class GimmickBase;
class Steelyard;

class ObjectManager;

class GameMain : public SceneBase
{
public:
	///�R���X�g���N�^
	GameMain(SceneManager& manager,bool continuation);
	//�f�X�g���N�^
	virtual ~GameMain();

	//�������p�֐�
	void init();
	//�폜�p�֐�
	void end();

	//�X�V
	void update(const InputState& input);
	//�`��
	void draw();

private:

	int makeScreenHandle_ = -1;
	int totalDeathNum_ = 0;					//�Q�[���J�n����̑����S��

	bool isContinuation_ = false;			//�������炩���߂��炩

	//������������Ȃ�
	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);

	//������������������Ȃ�
	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	VECTOR checkPoint_ = { 0.0f,0.0f, 0.0f };

	std::shared_ptr<Camera> camera_;			//�J�����̃V�F�A�[�h�|�C���^
	std::shared_ptr<Player> player_;			//�v���C���[�̃V�F�A�[�h�|�C���^
	std::shared_ptr<Broom> broom_;				//�����������������Ȃ�
	std::shared_ptr<DepthOfField> depthOfField_;//�����������������Ȃ�
	
	std::shared_ptr<ObjectManager> objManager_;

	std::list<std::shared_ptr<Model>> models_;//�Փ˔�����s���\��̃��f�����ЂƂ܂Ƃ߂ɂ���z��

	void (GameMain::* updateFunc_)(const InputState& input);		//�����o�֐��|�C���^

};

