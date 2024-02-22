#pragma once
#include <map>
#include <list>
#include <vector>
#include <string>
#include <memory>
#include <DxLib.h>

class Effect2D;

class EffectManager
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EffectManager();

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	/// <returns>インスタンス</returns>
	static EffectManager& GetInstance() {
		static EffectManager instance;
		return instance;
	}

	/// <summary>
	/// 画像の読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// エフェクトの追加
	/// </summary>
	/// <param name="name">エフェクト名</param>
	/// <param name="size">描画サイズ</param>
	/// <param name="pos">座標</param>
	void AddEffect(const std::string& name,const float size,const VECTOR& pos);

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EffectManager();

	EffectManager(const EffectManager&) = delete;
	void operator = (const EffectManager&) = delete;
private:
	int arrayHandle_[30] = {};

	std::map<std::string, std::vector<int>> handle_ = {};	//画像テーブル
	std::map<std::string,std::string> filepath_ = {};		//ファイルパス
	std::list<std::shared_ptr<Effect2D>> effect2D_ = {};	//effect
};

