#pragma once
#include "SceneBase.h"
#include <DxLib.h>
#include <memory>
#include <list>
#include <unordered_map>

class Camera;
class Player;
class Broom;
class DepthOfField;
class CheckCollisionModel;
class ObjectManager;
class ShotManager;
class Tutorial;

class GameMain : public SceneBase
{
public:
	///�R���X�g���N�^
	GameMain(SceneManager& manager);
	//�f�X�g���N�^
	virtual ~GameMain();

	//�������p�֐�
	void Init();
	//�폜�p�֐�
	void End();

	//�X�V
	void Update(const InputState& input);
	//�`��
	void Draw();

	//�I�u�W�F�N�g�̐���
	void ObjectGenerater();

private:

	int debugDrawTime_ = 0;
	int debugUpdateTime_ = 0;

	int skyHandle_ = -1;

	int makeScreenHandle_ = -1;
	int totalDeathNum_ = 0;					//�Q�[���J�n����̑����S��

	bool isFilterOn_ = false;				//�t�B���^�[���g�p���邩

	//������������Ȃ�
	void FadeInUpdate(const InputState& input);
	void NormalUpdate(const InputState& input);
	void FadeOutUpdate(const InputState& input);

	//������������������Ȃ�
	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	VECTOR checkPoint_ = { 0.0f,0.0f, 0.0f };

	std::shared_ptr<Camera> camera_;			//�J�����̃V�F�A�[�h�|�C���^
	std::shared_ptr<Player> player_;			//�v���C���[�̃V�F�A�[�h�|�C���^
	std::shared_ptr<Broom> broom_;				//�����������������Ȃ�
	std::shared_ptr<DepthOfField> depthOfField_;
	std::shared_ptr<ObjectManager> objManager_;
	std::shared_ptr<ShotManager> shotManager_;
	std::shared_ptr<Tutorial> tutorial_;
	
	std::shared_ptr<CheckCollisionModel> checkCollisionModel_;	//�Փ˔�����s���N���X�̃|�C���^

	void (GameMain::* updateFunc_)(const InputState& input);		//�����o�֐��|�C���^

};

