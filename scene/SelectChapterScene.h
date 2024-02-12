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
	void SlideInBook();
	void NormalUpdate();
	void FadeOutUpdate();
private:

	int textureHandle_[3] = {};
	int selectNum_ = 0;		//選択番号

	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	std::shared_ptr<Model> model_;			//モデルポインタ

	void(SelectChapterScene::* updateFunc_)();
};

