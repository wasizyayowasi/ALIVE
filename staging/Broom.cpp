#include "broom.h"
#include "../object/Camera.h"
#include "../util/game.h"
#include "../staging/Broom.h"

Broom::Broom()
{

	camera_ = std::make_shared<Camera>();

	gaussRation_ = 900;
	reductionScale_ = 8;
	reductionScreenWidth_ = Game::screen_width / reductionScale_;
	reductionScreenHeight_ = Game::screen_height / reductionScale_;

	normalScreen_ = MakeScreen(Game::screen_width,Game::screen_height,true);
	highBrightScreen_ = MakeScreen(Game::screen_width, Game::screen_height, true);
	reductioScaleScreen_ = MakeScreen(reductionScreenWidth_,reductionScreenHeight_, true);
	gaussScreen_ = MakeScreen(reductionScreenWidth_, reductionScreenHeight_, true);
	
}

Broom::~Broom()
{
	DeleteGraph(normalScreen_);
	DeleteGraph(highBrightScreen_);
	DeleteGraph(reductioScaleScreen_);
	DeleteGraph(gaussScreen_);
}

void Broom::writingScreenUpdate(VECTOR playerPos)
{
	//�ʏ�̕`�挋�ʂ��������ރX�N���[����`��Ώۂɂ���
	SetDrawScreen(normalScreen_);

	//��ʂ̃N���A
	ClearDrawScreen();

	camera_->FixedPointCamera(playerPos);
}

void Broom::graphFilterUpdate()
{
	//normalScreen_���獂�P�x�����݂̂𔲂��o�����摜�𓾂�
	GraphFilterBlt(normalScreen_, highBrightScreen_, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 230, true, 0x000000, 255);

	//highBrightScreen_��8����1�ɏk�������摜�𓾂�
	GraphFilterBlt(highBrightScreen_, reductioScaleScreen_, DX_GRAPH_FILTER_DOWN_SCALE, reductionScale_);

	//reductionScaleScreen_�ɃK�E�X�t�B���^�������{��
	GraphFilterBlt(reductioScaleScreen_, gaussScreen_, DX_GRAPH_FILTER_GAUSS, 16, gaussRation_);

	//�`��Ώۂ𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);
}

void Broom::update()
{

}

void Broom::draw()
{
	//�ʏ�̕`�挋�ʂ�`�悷��
	DrawGraph(0, 0, normalScreen_, false);

	//�`�惂�[�h���o�C���j�A�t�B���^�����O�ɂ���
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	//�`��u�����h���[�h�����Z�ɂ���
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	//���P�x�������k�����Ăڂ������摜����ʑS�̂�2��`�悷��(2��`�悷��̂͂�薾�邭���邽��)
	DrawExtendGraph(0, 0, Game::screen_width, Game::screen_height, gaussScreen_, false);
	DrawExtendGraph(0, 0, Game::screen_width, Game::screen_height, gaussScreen_, false);

	//�`��u�����h���[�h���u�����h�Ȃ��ɖ߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//�`�惂�[�h���j�A���X�g�ɖ߂�
	SetDrawMode(DX_DRAWMODE_NEAREST);

}


