#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>

class Model;
class Camera;
class ObjectManager;

class GameEnd : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	GameEnd(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~GameEnd();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �I��
	/// </summary>
	void End();

	//�X�V
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:
	void fadeInUpdate();
	void normalUpdate();
	void fadeOutUpdate();
private:

	int divisionNum_ = 0;												//������

	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	std::shared_ptr<Camera> camera_;									//�J�����̃|�C���^
	std::shared_ptr<Model> playerModel_;								//�v���C���[���f���̃|�C���^
	std::shared_ptr<ObjectManager> objManager_;							//�I�u�W�F�N�g�}�l�[�W���[�̃|�C���^

	std::vector<std::pair<bool,std::shared_ptr<Model>>> corpseModel_;	//���̃��f���̃|�C���^

	void (GameEnd::* updateFunc_)();

};

