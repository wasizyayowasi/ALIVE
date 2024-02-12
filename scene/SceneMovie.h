#pragma once
#include "SceneBase.h"

class SceneMovie : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	SceneMovie(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SceneMovie();

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
private:

	int currentMovieNum_ = 0;		//���݂̓���
	int movieHandle_[2] = {};		//����n���h��
};

