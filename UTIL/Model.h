#pragma once
#include "DxLib.h"

class Model
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="fileName">モデルが入っているファイルパス</param>
	Model(const char* fileName);
	//duplicate用コンストラクタ
	Model(int orgModel);

	//デストラクタ
	virtual ~Model();

	/// <summary>
	/// collisionフレームをcollisionとして使う
	/// </summary>
	/// <param name="isUse">使用するか</param>
	/// <param name="isNeedUpdate">更新が必要か</param>
	void setUseCollision(bool isUse, bool isNeedUpdate);

	//更新
	void update();
	//描画
	void draw();

	/// <summary>
	/// モデルを取得する
	/// </summary>
	/// <returns>モデルハンドルを返す</returns>
	int getModelHandle() const { return modelHandle_; }
	/// <summary>
	/// collisionフレームを取得する
	/// </summary>
	/// <returns>collisionフレームを返す</returns>
	int getColFrameIndex() const { return colFrameIndex_; }
	/// <summary>
	/// ポジションを取得する
	/// </summary>
	/// <returns>ポジションを返す</returns>
	VECTOR getPos() const { return pos_; }
	/// <summary>
	/// アニメーションの再生時間を取得する
	/// </summary>
	/// <returns>一アニメーションの再生時間</returns>
	float getAnimTotalTime()const { return animNext_.totalTime; }

	/// <summary>
	/// ポジションを設定する
	/// </summary>
	/// <param name="pos">ポジション</param>
	void setPos(VECTOR pos);
	/// <summary>
	/// 角度を設定する
	/// </summary>
	/// <param name="rot">角度</param>
	void setRot(VECTOR rot);
	/// <summary>
	/// サイズを設定する
	/// </summary>
	/// <param name="scale">拡縮率</param>
	void setScale(VECTOR scale);
	/// <summary>
	/// collisionとして使いたいフレームを指定する
	/// </summary>
	/// <param name="collFrameName">collisionとして使いたいフレーム名。なければ空</param>
	void setCollFrame(const char* collFrameName = "");

	/// <summary>
	/// アニメーションを設定する
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	/// <param name="isLoop">ループ再生するか</param>
	/// <param name="IsForceChange">強制的に変更するか</param>
	void setAnimation(int animNo,bool isLoop,bool IsForceChange);

	/// <summary>
	/// アニメーションを変更する
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	/// <param name="isLoop">ループ再生するか</param>
	/// <param name="isForceChange">強制的に変更するか</param>
	/// <param name="changeFrame">アニメーションを変更する際に何秒かけて変化するのか</param>
	void changeAnimation(int animNo, bool isLoop, bool isForceChange, int changeFrame);

	/// <summary>
	/// アニメーションの終わりを取得する
	/// </summary>
	/// <returns>アニメーションが終わったか</returns>
	bool isAnimEnd();

	/// <summary>
	/// アニメーション番号の最後のフレームを設定する
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	void setAnimEndFrame(int animNo);

	/// <summary>
	/// 指定したフレームの座標を取得する
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	/// <param name="frameName">取得したいアニメーション名</param>
	/// <returns>座標</returns>
	VECTOR getAnimFrameLocalPosition(int animNo,const char* frameName);

private:
	struct AnimData {
		int animNo;
		int attachNo;
		float totalTime;
		bool isLoop;
	};
private:
	/// <summary>
	/// アニメーションデータを初期化する
	/// </summary>
	/// <param name="anim"></param>
	void clearAnimData(AnimData& anim);

	/// <summary>
	/// アニメーションの更新
	/// </summary>
	/// <param name="anim"></param>
	/// <param name="dt"></param>
	void updateAnim(AnimData anim, float dt = 1.0f);

	//アニメーションのブレンド
	void updateAnimBlendRate();
private:
	int modelHandle_ = -1;

	bool isUseCollision_ = false;

	bool isUpdateCollsion_ = false;

	int colFrameIndex_ = 0;

	AnimData animPrev_;
	AnimData animNext_;

	int animChangeFrame_ = 0;
	int animChangeFrameTotal_ = 0;

	VECTOR pos_;

};

