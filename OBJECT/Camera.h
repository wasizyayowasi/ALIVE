#pragma once
#include <DxLib.h>

class Camera
{
public:
	//�R���X�g���N�^
	Camera(VECTOR pos);
	//�f�X�g���N�^
	virtual ~Camera();

	//�������p
	void Init(VECTOR targetPos);

	//�X�V
	void Update(VECTOR playerPos, float playerHeight);

	/// <summary>
	/// �v���C���[��ǐՂ���J�����̍X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	void TrackingCameraUpdate(VECTOR playerPos, float playerHeight);

	/// <summary>
	/// ��_�J����
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	void FixedPointCamera(VECTOR playerPos, float playerHeight);

	/// <summary>
	/// �J�����̒����_����炷
	/// </summary>
	void ChangeOfFocus();

	VECTOR GetPos() { return pos_; }
	VECTOR GetTarget() { return cameraTarget_; }

	void tempdraw();

private:

	/// <summary>
	/// �v���C���[��X���W��ǐ�
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	float TrackingPosX(VECTOR playerPos);

	/// <summary>
	/// �v���C���[��Y���W��ǐ�
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	float TrackingPosY(VECTOR playerPos, float playerHeight);

	/// <summary>
	/// �v���C���[��Z���W��ǐ�
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	float TrackingPozZ(VECTOR playerPos);

private:

	int i = 0;
	float moveVecZ = 0.0f;
	float moveVecY = 0.0f;
	float moveVecX = 0.0f;

	DINPUT_JOYSTATE input_ = {};

	VECTOR pos_ = {};							//�J�����̃|�W�V����
	VECTOR cameraTarget_ = {0, 0, 0};			//�J�����̃^�[�Q�b�g
	VECTOR initPos_ = {};						//�����|�W�V����

	void(Camera::* updateFunc_)(VECTOR pos, float height);

};

