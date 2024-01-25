#include "CrankScaffold.h"
#include "ManualCrank.h"

#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/Util.h"

#include "../object/Player.h"

namespace {
	constexpr float ascent_limit = 200.0f;
}

//コンストラクタ
CrankScaffold::CrankScaffold(int handle, LoadObjectInfo objInfo) : GimmickBase(handle, objInfo)
{
	//初期ポジションの初期化
	initPos_ = objInfo.pos;

	//衝突判定用フレームの設定
	model_->SetUseCollision(true, false);

	//objInfoの名前から末尾の数値を取得する
	int num = StrUtil::GetNumberFromString(objInfo.name, ".");
	//上記の数値と第一引数の文字列を連結させた文字列を取得する
	std::string name = StrUtil::GetConcatenateNumAndStrings("Crank", ".", num);

	//クランクの配置データを取得する
	auto info = ExternalFile::GetInstance().GetSpecifiedGimmickInfo(initPos_, name.c_str());
	//インスタンス化
	crank_ = std::make_shared<ManualCrank>(info);

	//上昇ベクトルの初期化
	upVec_ = ascent_limit / crank_->GetMaxRotZ();
}

//デストラクタ
CrankScaffold::~CrankScaffold()
{
}

//更新
void CrankScaffold::Update(Player& player)
{
	//クランクとプレイヤーの衝突判定を行い
	//当たっていた場合プレイヤーに当たっていたクラスのポインターを
	//セットする
	if (crank_->HitCollPlayer(player)) {
		player.SetCrankPointer(crank_);
	}

	//移動
	pos_.y = crank_->GetRotZ()* upVec_ + initPos_.y;

	//ポジションのセット
	model_->SetPos(pos_);
}

//描画
void CrankScaffold::Draw()
{
	//モデルの描画
	model_->Draw();
	//クランクの描画
	crank_->Draw();
}

//衝突判定を行うモデルの追加
std::shared_ptr<Model> CrankScaffold::AddCollModel()
{
	return nullptr;
}
