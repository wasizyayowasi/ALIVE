#include "SignBoardEnemy.h"

#include "Player.h"

#include "../util/Model.h"
#include "../util/FontsManager.h"

SignBoardEnemy::SignBoardEnemy(int handle, Material materialType, LoadObjectInfo objInfo):EnemyBase(handle,materialType,objInfo)
{
	//マテリアルの色を変える
	MV1SetMaterialDifColor(model_->GetModelHandle(), 8, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
}

SignBoardEnemy::~SignBoardEnemy()
{
}

void SignBoardEnemy::Update(Player& player)
{
	deathCount_ = player.GetDeathCount();
}

void SignBoardEnemy::Draw()
{
	model_->Draw();

	VECTOR framePos = model_->GetFrameLocalPosition("display");

	VECTOR screenPos = ConvWorldPosToScreenPos(framePos);

	DrawFormatStringFToHandle(screenPos.x, screenPos.y, 0x000000, FontsManager::GetInstance().GetFontHandle("ピグモ 0021"), "%d", deathCount_);
}
