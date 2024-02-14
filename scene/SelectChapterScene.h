#pragma once
#include "SceneBase.h"
#include <memory>

class Model;

class SelectChapterScene : public SceneBase
{
private:

	enum class BookAnim {
		normal = 3,
		open,
		close,
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	SelectChapterScene(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SelectChapterScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 終了
	/// </summary>
	void End()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// チャプターを変更する
	/// </summary>
	void ChangeChapter();

private:
	/// <summary>
	/// 本がスライドインしてくる
	/// </summary>
	void SlideInBook();

	/// <summary>
	/// 通常の更新
	/// </summary>
	void NormalUpdate();

	/// <summary>
	/// 本がスライドアウトしていく
	/// </summary>
	void SlideOutBook();

	/// <summary>
	/// フェードアウト
	/// </summary>
	void FadeOutUpdate();
private:

	int textureHandle_[3] = {};						//テクスチャハンドル
	int selectNum_ = 0;								//選択番号

	float elapsedTime_ = 0;							//経過時間
	float targetPosX_ = 0.0f;						//目標のz座標

	static constexpr int fadeInterval_ = 60;		//フェードインターバル
	int fadeTimer_ = 0;								//フェード時間
	int fadeValue_ = 0;								//フェードの値
	int fadeColor_ = 0x000000;						//フェードの色

	std::shared_ptr<Model> model_;					//モデルポインタ

	void(SelectChapterScene::* updateFunc_)();
};

