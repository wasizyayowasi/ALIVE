#include "Shot.h"

#include "Player.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/SoundManager.h"
#include "../util/EffectManager.h"

namespace {
	//�������C��
	constexpr float vanishing_line = 1500.0f;

	//�d��
	constexpr float gravity = -1.0f;
}

Shot::Shot(const int handle, const  VECTOR initPos, const  VECTOR moveVec)
{
	//���f���̃C���X�^���X��
	model_ = std::make_shared<Model>(handle,Material::Stone);

	//�T�C�Y�̒���
	float scale = 20.0f;
	model_->SetScale(VGet(scale, scale, scale));

	//�|�W�V����
	pos_ = initPos;

	//�����ʒu
	initialisePos_ = initPos;

	//�x�N�g��
	moveVec_ = moveVec;

	//���݂��Ă���t���O
	isEnable_ = true;
}

Shot::~Shot()
{
}

void Shot::Update()
{
	//�|�W�V�����̍X�V
	pos_ = VAdd(pos_, moveVec_);

	//���f���̃|�W�V������ݒ�
	model_->SetPos(pos_);

	//�����T�C�Y
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(initialisePos_, pos_);

	//�����T�C�Y����胉�C���𒴂���Ƒ��݂��Ă���t���O��false�ɂ���
	if (distanceSize > vanishing_line) {
		isEnable_ = false;
	}

	//���f���̍X�V
	model_->Update();
}

void Shot::Draw()
{
	model_->Draw();
}

void Shot::HitCheck(Player& player)
{
	//�v���C���[�̍��W
	VECTOR playerPos = player.GetStatus().pos;

	//�v���C���[�̍���
	float playerHeight = player.GetStatus().height;

	//�v���C���[�ƃV���b�g�̓����蔻��
	//�J�v�Z���Ƌ���
	bool hit = HitCheck_Sphere_Capsule(pos_, 50.0f, playerPos, VGet(playerPos.x, playerPos.y + playerHeight, playerPos.z),20.0f);

	//�Փ˂�����m�b�N�o�b�N�x�N�g�������
	// ����炵��
	//�v���C���[�̃x�N�g���ɐݒ肵�A���݂���t���O��false�ɂ���
	if (hit) {
		VECTOR nockBack = VScale(VNorm(moveVec_),10.0f);
		player.BulletHitMe(nockBack);

		//�T�E���h��炷
		SoundManager::GetInstance().Set3DSoundInfo(playerPos, 1200.0f, "hit");
		SoundManager::GetInstance().PlaySE("hit");

		//�G�t�F�N�g���o��
		EffectManager::GetInstance().AddEffect("BigHit",100.0f, pos_);

		isEnable_ = false;
	}
}
