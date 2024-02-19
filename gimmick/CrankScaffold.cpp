#include "CrankScaffold.h"
#include "ManualCrank.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"

#include "../object/Player.h"

//コンストラクタ
CrankScaffold::CrankScaffold(const int handle, const Material materialType, const LoadObjectInfo objInfo) : GimmickBase(handle, materialType, objInfo)
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//objInfoの名前から末尾の数値を取得する
	int num = StrUtil::GetNumberFromString(objInfo.name, ".");

	//上記の数値と第一引数の文字列を連結させた文字列を取得する
	std::string name = StrUtil::GetConcatenateNumAndStrings("Crank", ".", num);

	//クランクの配置データを取得する
	auto info = file.GetSpecifiedGimmickInfo(initPos_, name.c_str());

	//インスタンス化
	crank_ = std::make_shared<ManualCrank>(info);

	//足場の上限のポジション
	name = StrUtil::GetConcatenateNumAndStrings("CrankUpperLimit", ".", num);
	VECTOR upperLimitPos = file.GetSpecifiedGimmickInfo(initPos_, name.c_str()).pos;

	//足場の下限のポジション
	name = StrUtil::GetConcatenateNumAndStrings("CrankLowerLimit", ".", num);
	VECTOR lowerLimitPos = file.GetSpecifiedGimmickInfo(initPos_, name.c_str()).pos;

	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(upperLimitPos, lowerLimitPos);

	//初期ポジションの初期化
	initPos_ = objInfo.pos;

	//衝突判定用フレームの設定
	model_->SetUseCollision(true, false);

	//上昇ベクトルの初期化
	upVec_ = distanceSize / crank_->GetMaxRotZ();
}

//デストラクタ
CrankScaffold::~CrankScaffold()
{
}

//更新
void CrankScaffold::Update(Player& player)
{
	//短縮化
	auto& sound = SoundManager::GetInstance();

	//クランクとプレイヤーの衝突判定を行い
	//当たっていた場合プレイヤーに当たっていたクラスのポインターを
	//セットする
	if (crank_->HitCollPlayer(player)) {
		player.SetCrankPointer(crank_);
	}

	float nowRotZ = crank_->GetRotZ();

	if (oldRotZ_ != nowRotZ) {
		if (crank_->GetMaxRotZ() == crank_->GetRotZ() || crank_->GetRotZ() == 0) {
			if (!sound.CheckSoundFile("stopCrank")) {
				sound.Set3DSoundInfo(pos_, 1500.0f, "stopCrank");
				sound.PlaySE("stopCrank");
			}
		}
	}

	//移動
	pos_.y = crank_->GetRotZ()* upVec_ + initPos_.y;

	//ポジションのセット
	model_->SetPos(pos_);

	//Z回転の値を残しておく
	oldRotZ_ = crank_->GetRotZ();
}

//描画
void CrankScaffold::Draw()
{
	//モデルの描画
	model_->Draw();
	//クランクの描画
	crank_->Draw();
}