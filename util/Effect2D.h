#pragma once
#include <DxLib.h>

class Effect2D
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="filepath">�t�@�C���p�X</param>
	/// <param name="numX">���̕�����</param>
	/// <param name="numY">�c�̕�����</param>
	/// <param name="graphSizeX">1�̉摜�̉��̃T�C�Y</param>
	/// <param name="graphSizeY">1�̉摜�̏c�̃T�C�Y</param>
	/// <param name="pos">�`��ꏊ</param>
	Effect2D(const char* const filepath, int numX, int numY, int graphSizeX, int graphSizeY, float size, VECTOR pos);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Effect2D();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �摜��3D��Ԃɕ`�悷��
	/// </summary>
	void DrawBillboard();

	/// <summary>
	/// ���݂��Ă��邩�ǂ����擾����
	/// </summary>
	/// <returns>true�F���Ă���@false�F���Ă��Ȃ�</returns>
	bool IsEnable() { return isEnabled_; }
private:

	int elapsedTime_ = 0;			//�o�ߎ���
	int currentNum_ = 0;			//���݂̔ԍ�
	int totalNum_ = 0;				//�������̐�

	int handle_[99] = {};			//�摜�p�̔z��

	float drawSize_ = 0.0f;			//�`��T�C�Y

	bool isEnabled_ = false;		//���݂��Ă��邩

	VECTOR drawPos_ = {};			//�`��ꏊ
};

