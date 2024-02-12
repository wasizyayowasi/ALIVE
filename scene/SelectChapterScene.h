#pragma once
#include "SceneBase.h"
#include <memory>

class Model;

class SelectChapterScene : public SceneBase
{
private:

	enum class BookAnim {
		normal = 3,
		open,
		close,
	};

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	SelectChapterScene(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SelectChapterScene();

	/// <summary>
	/// ������
	/// </summary>
	void Init()override;

	/// <summary>
	/// �I��
	/// </summary>
	void End()override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �`���v�^�[��ύX����
	/// </summary>
	void ChangeChapter();

private:
	void SlideInBook();
	void NormalUpdate();
	void FadeOutUpdate();
private:

	int textureHandle_[3] = {};
	int selectNum_ = 0;		//�I��ԍ�

	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	std::shared_ptr<Model> model_;			//���f���|�C���^

	void(SelectChapterScene::* updateFunc_)();
};

