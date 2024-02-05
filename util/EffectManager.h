#pragma once
#include <DxLib.h>
#include <map>
#include <list>
#include <string>
#include <memory>

class Effect2D;

class EffectManager
{
private:

	struct EffectInfo {
		int numX;			//横の分割数
		int numY;			//縦の分割数
		int chipSizeX;		//横のチップサイズ
		int chipSizeY;		//縦のチップサイズ
		float size;			//描画サイズ
	};

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EffectManager();

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	/// <returns></returns>
	static EffectManager& GetInstance() {
		static EffectManager instance;
		return instance;
	}

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
	/// <param name="pos">座標</param>
	void AddEffect(std::string name, VECTOR pos);

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EffectManager();

	EffectManager(const EffectManager&) = delete;
	void operator = (const EffectManager&) = delete;
private:
	std::map<std::string,std::string> filepath_ = {};		//ファイルパス
	std::map<std::string,EffectInfo> handleInfo_ = {};		//effect情報
	std::list<std::shared_ptr<Effect2D>> effect2D_ = {};	//effect
};

