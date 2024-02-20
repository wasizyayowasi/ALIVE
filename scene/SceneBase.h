#pragma once

class SceneManager;

class SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	SceneBase(SceneManager& manager) : manager_(manager){}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneBase() {};
	
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void End() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;
protected:
	SceneManager& manager_;			//シーンマネージャーの参照
};

