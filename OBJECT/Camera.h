#pragma once
#include <DxLib.h>

class Camera
{
public:
	//�R���X�g���N�^
	Camera(VECTOR pos);
	//�f�X�g���N�^
	virtual ~Camera();

	/// <summary>
	/// �J�����̏�����
	/// </summary>
	/// <param name="targetPos">�J����������ʒu</param>
	void Init(VECTOR targetPos);

	/// <summary>
	/// �J�����̍X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <param name="playerHeight">�v���C���[�̐g��</param>
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

	/// <summary>
	/// �ڕW�̃|�W�V�����܂ňړ�����
	/// </summary>
	void EasingMoveCamera();

	/// <summary>
	/// �J�����̃|�W�V�������擾����
	/// </summary>
	/// <returns>�J�����̍��W</returns>
	VECTOR GetPos() { return pos_; }

	/// <summary>
	/// �J����������ʒu���擾����
	/// </summary>
	/// <returns>�J����������ʒu</returns>
	VECTOR GetTarget() { return cameraViewingPos_; }

	/// <summary>
	/// �J�������ڕW�Ƃ�����W��ݒ肷��
	/// </summary>
	/// <param name="targetPos">�ڕW�ʒu</param>
	void SetCameraTargetPos(VECTOR targetPos);

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

	float elapsedTime_ = 0.0f;

	DINPUT_JOYSTATE input_ = {};

	VECTOR initPos_ = {};						//�����|�W�V����
	VECTOR pos_ = {};							//�J�����̃|�W�V����
	VECTOR cameraViewingPos_ = {};				//�J����������ʒu
	VECTOR cameraTargetPos_ = {};				//�J�����̖ڕW�ʒu

	void(Camera::* updateFunc_)(VECTOR pos, float height);

};

