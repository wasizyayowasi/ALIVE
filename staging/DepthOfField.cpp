#include "DepthOfField.h"
#include "../util/game.h"

DepthOfField::DepthOfField()
{

	

	textureWidth = Game::screen_width;
	textureHeight = Game::screen_height;

	normalTexture = MakeScreen(textureWidth, textureHeight, true);
	weakVertigoTexture = MakeScreen(textureWidth, textureHeight, true);
	strongVertigoTexture = MakeScreen(textureWidth, textureHeight, true);

	SetCreateDrawValidGraphChannelNum(1);
	depthTexture = MakeScreen(textureWidth, textureHeight, true);
	SetCreateDrawValidGraphChannelNum(4);

	//	depthNormalMeshVS = LoadVertexShader("");
	//	depthSkinMeshVS = LoadVertexShader("");
	//	depthPS = LoadPixelShader("");
	depthOfFiledPS = LoadPixelShader("DATA/shader/DepthOfField.pso");

}

DepthOfField::~DepthOfField()
{
	//	DeleteShader(depthNormalMeshVS);
	//	DeleteShader(depthSkinMeshVS);
	//	DeleteShader(depthPS);
	//DeleteShader(depthOfFiledPS);
}

void DepthOfField::drawScreen(int ScreenGraphHandle, bool UseDepthShader)
{

}

void DepthOfField::depthOfFieldInit()
{
	//��Ԕ͈͂ƃt�H�[�J�X�̑�����
	depthOfFieldTotalSize = depthOfFieldInterpSize * 2.0f + depthOfFieldFocusSize;

	//�Ï�`��p�e�N�X�`���Ƀ��f����ʏ�`��
	drawScreen(normalTexture, false);

	//�ʏ�`��̌��ʂɝ򂵂��������摜��ۑ�
	GraphFilterRectBlt(normalTexture, weakVertigoTexture, 0, 0, Game::screen_width, Game::screen_height, 0, 0, DX_GRAPH_FILTER_GAUSS, 16, 200);

	//�򂵂��������ꑝ�ɂ���ɝ򂵂������Ĕ�z��
	GraphFilterRectBlt(weakVertigoTexture, strongVertigoTexture, 0, 0, Game::screen_width, Game::screen_height, 0, 0, DX_GRAPH_FILTER_GAUSS, 16, 200);

	//�[�x�l�`��p�e�N�X�`���ɐ[�x�l��`��
	drawScreen(depthTexture,false);

	//�`���𗠉�ʂɕύX
	SetDrawScreen(DX_SCREEN_BACK);

}

void DepthOfField::draw()
{
	SetUseTextureToShader(0, normalTexture);
	SetUseTextureToShader(1, weakVertigoTexture);
	SetUseTextureToShader(2, strongVertigoTexture);
	SetUseTextureToShader(3, depthTexture);

	SetUsePixelShader(depthOfFiledPS);

	//��ʊE�[�x�̂ڂ����Ȃ��̊J�n�ʒu���v�Z
	unblurredRange.x = depthOfFieldInterpSize / depthOfFieldTotalSize;

	//��ʊE�[�x�̂ڂ����Ȃ��̏I���ʒu���v�Z
	unblurredRange.y = (depthOfFieldInterpSize + depthOfFieldFocusSize) / depthOfFieldTotalSize;

	SetPSConstF(0, unblurredRange);

	//��ʑS�̂ɕ`�悷�钸�_�̏���
	vertex_[0].pos = VGet(0.0f,0.0f,0.0f);
	vertex_[1].pos = VGet(Game::screen_width,0.0f,0.0f);
	vertex_[2].pos = VGet(0.0f,Game::screen_height,0.0f);
	vertex_[3].pos = VGet(Game::screen_width,Game::screen_height,0.0f);

	vertex_[0].rhw = 1.0f;
	vertex_[1].rhw = 1.0f;
	vertex_[2].rhw = 1.0f;
	vertex_[3].rhw = 1.0f;

	rightU_ = Game::screen_width / textureWidth;
	bottomV_ = Game::screen_height / textureWidth;

	vertex_[0].u = 0.0f;
	vertex_[1].u = rightU_;
	vertex_[2].u = 0.0f;
	vertex_[3].u = rightU_;

	vertex_[0].v = 0.0f;
	vertex_[1].v = 0.0f;
	vertex_[2].v = bottomV_;
	vertex_[3].v = bottomV_;

	vertex_[4] = vertex_[2];
	vertex_[5] = vertex_[1];

	DrawPolygon2DToShader(vertex_, 2);

}