#pragma once
#include <DxLib.h>

class InputState;

class Camera
{
public:
	//�R���X�g���N�^
	Camera();
	//�f�X�g���N�^
	virtual ~Camera();

	//�������p
	void Init();

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
	/// <param name="input">���͏��</param>
	void ChangeOfFocus(const InputState& input);

	VECTOR GetPos() { return cameraPos_; }
	VECTOR GetTarget() { return cameraTarget_; }

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
	float cameraTargetPosZ = 0.0f;
	float moveVecZ = 0.0f;
	float moveVecY = 0.0f;
	float moveVecX = 0.0f;

	DINPUT_JOYSTATE input_ = {};

	VECTOR cameraPos_;									//�J�����̃|�W�V����
	VECTOR cameraTarget_ = {0, 0, 0};					//�J�����̃^�[�Q�b�g

	void(Camera::* updateFunc_)(VECTOR pos, float height);

};

