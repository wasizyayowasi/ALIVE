#pragma once
#include "../object/ObjectData.h"
#include <string>
#include <DxLib.h>
#include <unordered_map>

class ModelManager
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ModelManager();

	/// <summary>
	/// インスタンス作成
	/// </summary>
	/// <returns>インスタンス</returns>
	static ModelManager& GetInstance() {
		static ModelManager instance;
		return instance;
	}

	/// <summary>
	/// モデルをロードする
	/// </summary>
	void LoadModel();

	/// <summary>
	/// 3Dモデルハンドルを取得する
	/// </summary>
	/// <returns>モデルハンドル</returns>
	int GetModelHandle(const std::string& name);
private:

	std::unordered_map<std::string, int> modelHandle_;		//モデルハンドルテーブル

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ModelManager() = default;

	ModelManager(const ModelManager&) = delete;
	void operator = (const ModelManager&) = delete;
};

