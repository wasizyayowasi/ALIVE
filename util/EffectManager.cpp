#include "EffectManager.h"
#include "Effect2D.h"

#include "../util/ExternalFile.h"

#include <algorithm>

namespace {
	//ファイルパス
	const char* const hit_effect_filepath = "data/effect/BigHit.png";
	const char* const smoke_effect_filepath = "data/effect/Smoke.png";
	const char* const leaves_effect_filepath = "data/effect/Leaves.png";
}

//コンストラクタ
EffectManager::EffectManager()
{
}

//デストラクタ
EffectManager::~EffectManager()
{
	for (auto& table : handle_)
	{
		for (auto& graph : table.second)
		{
			DeleteGraph(graph);
		}
	}

	for (auto& graph : arrayHandle_) {
		DeleteGraph(graph);
	}
}

//画像の読み込み
void EffectManager::Load()
{
	//分割画像の読み込み
	for (auto& place : ExternalFile::GetInstance().GetDivGraphData())
	{
		for (auto& info : place.second)
		{
			//パスの作成
			std::string path = "data/" + place.first + "/" + info.name + ".png";

			//総分割数の作成
			int totalDivNum = info.divXNum * info.divYNum;

			//画像の分割読み込み
			LoadDivGraph(path.c_str(), totalDivNum, info.divXNum, info.divYNum, info.divXSize, info.divYSize, arrayHandle_);

			for (int i = 0; i < totalDivNum; i++)
			{
				handle_[info.name].push_back(arrayHandle_[i]);
				arrayHandle_[i] = 0;
			}
		}
	}
}

//更新
void EffectManager::Update()
{
	effect2D_.remove_if([](std::shared_ptr<Effect2D> effectPoint) {return !effectPoint->IsEnable(); });

	//更新
	for (auto& effect : effect2D_) {
		effect->Update();
	}
}

//描画
void EffectManager::Draw()
{
	//描画
	for (auto& effect : effect2D_) {
		effect->DrawBillboard();
	}
}

//エフェクトの追加
void EffectManager::AddEffect(const std::string& name, const float size, const VECTOR& pos)
{
	effect2D_.push_back(std::make_shared<Effect2D>(handle_[name], size, pos));
}