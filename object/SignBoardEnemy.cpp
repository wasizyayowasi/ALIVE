#include "SignBoardEnemy.h"

#include "Player.h"

#include "../util/Model.h"
#include "../util/FontsManager.h"
#include "../util/ModelManager.h"

namespace
{
	//色を変えるマテリアルの番号
	constexpr int change_material_num = 8;
}

//コンストラクタ
SignBoardEnemy::SignBoardEnemy(const int handle, const Material materialType, const LoadObjectInfo& objInfo):EnemyBase(handle,materialType,objInfo)
{
	auto& model = ModelManager::GetInstance();

	//マテリアルの色を変える
	MV1SetMaterialDifColor(model_->GetModelHandle(), change_material_num, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
	numModel_[0] = MV1DuplicateModel(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Number)].name));
	numModel_[1] = MV1DuplicateModel(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Number)].name));
}

//デストラクタ
SignBoardEnemy::~SignBoardEnemy()
{
}

//更新
void SignBoardEnemy::Update(Player& player)
{
	deathCount_ = player.GetDeathCount();
}

//描画
void SignBoardEnemy::Draw()
{
	model_->Draw();

	//0～9の数を出力するため
	int ten = 10;

	//数を表示する場所を取得する
	VECTOR display1LocalPos = model_->GetFrameLocalPosition("display1");
	VECTOR display2LocalPos = model_->GetFrameLocalPosition("display2");

	//指定のフレームインデックスを取得
	int display1FrameNum = MV1SearchFrame(numModel_[0], std::to_string(deathCount_ / ten).c_str());
	int display2FrameNum = MV1SearchFrame(numModel_[1], std::to_string(deathCount_ % ten).c_str());

	//弧度法の回転Z
	float radianZ = numRot_.z * DX_PI_F / 180.0f;

	//回転行列
	MATRIX rotMtx = MGetRotZ(radianZ);

	//拡縮行列
	MATRIX scaleMtx = MGetScale(numScale_);

	//平行移動行列
	MATRIX display1TransMtx = MGetTranslate(display1LocalPos);
	MATRIX display2TransMtx = MGetTranslate(display2LocalPos);

	//回転と拡縮行列の乗算
	MATRIX display1Mtx = MMult(rotMtx, scaleMtx);
	MATRIX display2Mtx = MMult(rotMtx, scaleMtx);

	//回転と拡縮行列と平行移動行列の乗算
	display1Mtx = MMult(display1Mtx, display1TransMtx);
	display2Mtx = MMult(display2Mtx, display2TransMtx);

	//フレームに行列を使う
	MV1SetFrameUserLocalMatrix(numModel_[0], display1FrameNum, display1Mtx);
	MV1SetFrameUserLocalMatrix(numModel_[1], display2FrameNum, display2Mtx);

	MV1DrawFrame(numModel_[0], display1FrameNum);
	MV1DrawFrame(numModel_[1], display2FrameNum);
}
