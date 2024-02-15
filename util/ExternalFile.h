#pragma once
#include "../object/PlayerData.h"
#include "../object/ObjectData.h"
#include <list>
#include <deque>
#include <string>
#include <DxLib.h>
#include <unordered_map>

class ExternalFile
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ExternalFile();

	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns></returns>
	static ExternalFile& GetInstance() {
		static ExternalFile instance;
		return instance;
	}

	/// <summary>
	/// ファイルをまとめて読み込む
	/// </summary>
	void LoadFile();

	/// <summary>
	/// 配置データを読み込む
	/// </summary>
	void LoadArrangementData();

	/// <summary>
	/// ファイルを読み込む
	/// </summary>
	void LoadFileHandle(std::string name);

	/// <summary>
	/// 特定のギミックの配置情報を取得する
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <returns></returns>
	LoadObjectInfo GetSpecifiedGimmickInfo(VECTOR objPos, const char* const name);

	/// <summary>
	/// カメラが特殊な動きを行う印(オブジェクト)が
	/// どこにあるかの配置データを取得する
	/// </summary>
	/// <returns></returns>
	LoadObjectInfo GetCameraGimmickInfo(VECTOR playerPos,const char* const name);

	/// <summary>
	/// 指定した名前のオブジェクト配置データを返す
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <returns>オブジェクトのデータ</returns>
	LoadObjectInfo GetSpecifiedInfo(const char* const stage,const char* const name);

	/// <summary>
	/// エネミーの配置データを取得する
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <returns>配置データ</returns>
	std::list<LoadObjectInfo> GetEnemyInfo(VECTOR playerPos);

	/// <summary>
	/// プレイヤーの開始位置のデータを取得する
	/// </summary>
	/// <param name="name">開始位置の名前</param>
	/// <returns>配置データ</returns>
	VECTOR GetStartPos(std::string name);

	/// <summary>
	/// チュートリアルを表示するポイントの配置データを取得する
	/// </summary>
	/// <param name="pos">プレイヤーのポジション</param>
	/// <returns>配置データ</returns>
	LoadObjectInfo GetTutorialObjInfo(VECTOR pos);

	/// <summary>
	/// セーブデータの書き出し
	/// </summary>
	/// <param name="num">死亡回数</param>
	void SaveDataRewriteInfo(int num);

	/// <summary>
	/// プレイヤーに関する情報を取得する
	/// </summary>
	/// <returns>playerData</returns>
	PlayerInfo GetPlayerInfo() { return player_; }

	/// <summary>
	/// 開始場所の名前を設定する
	/// </summary>
	/// <param name="name">開始場所の名前</param>
	void SetStartName(std::string name);

	/// <summary>
	/// 開始場所の名前を取得する
	/// </summary>
	/// <returns>開始場所の名前</returns>
	std::string GetStartName() { return startPosName_; }

	/// <summary>
	/// savedataを取得する
	/// </summary>
	/// <returns>過去の死亡回数</returns>
	std::deque<int> GetTotalDeathNum() { return pastTotalDeathNum_; }

	/// <summary>
	/// オブジェクトの配置情報を取得
	/// </summary>
	/// <returns>配置データ</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetGimmickInfo() { return loadGimmickInfo_; }

	/// <summary>
	/// メインステージオブジェクトの配置、回転率、拡縮率を取得する
	/// </summary>
	/// <returns>オブジェクトの配置データなどをまとめた変数</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetLoadMainStageObjectInfo() { return loadMainStageObjInfo_; }

	/// <summary>
	/// オープニングステージオブジェクトの配置、回転率、拡縮率を取得する
	/// </summary>
	/// <returns>オブジェクトの配置データなどをまとめた変数</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetLoadOpeningStageObjectInfo() { return loadOpeningStageObjInfo_; }

	/// <summary>
	/// エンディングシーンオブジェクトの配置、回転率、拡縮率を取得する
	/// </summary>
	/// <returns>オブジェクトの配置データなどをまとめた変数</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetLoadEndingStageObjectInfo() { return loadEndStageObjInfo_; }

	/// <summary>
	/// カメラの座標データを取得する
	/// </summary>
	/// <param name="name">名前</param>
	/// <returns>座標データ</returns>
	VECTOR GetCameraTargetPos(std::string name);

	/// <summary>
	/// 指定UIの配置座標を取得する
	/// </summary>
	/// <param name="name">取得したいUIの名前</param>
	/// <returns>座標</returns>
	VECTOR GetUIPos(std::string name);

	/// <summary>
	/// 死んだ回数をセットする
	/// </summary>
	/// <param name="num"></param>
	void SetDeathCount(int num);
private:

	/// <summary>
	/// プレイヤーのステータス情報を書き出す 
	/// </summary>
	void RewritePlayerInfo();

	/// <summary>
	/// プレイヤーのステータスに関する情報を読み込む
	/// </summary>
	/// <param name="filename">ファイルの名前</param>
	void LoadPlayerInfo(const char* const filename);

	/// <summary>
	/// セーブデータを読み込む
	/// </summary>
	/// <param name="filename">ファイルの名前</param>
	void LoadSaveDataInfo(const char* const filename);

	/// <summary>
	/// オブジェクトの配置情報を読み込みlistに格納する
	/// </summary>
	/// <param name="filename">ファイルの名前</param>
	/// <param name="dataTable">データテーブル</param>
	void LoadObjectDataList(std::string name, std::unordered_map<std::string, std::list<LoadObjectInfo>>& dataTable);

	/// <summary>
	/// オブジェクトの配置情報を読み込む
	/// </summary>
	/// /// <param name="filename">ファイルの名前</param>
	/// <param name="dataTable">データテーブル</param>
	void LoadObjectData(std::string name, std::unordered_map<std::string, LoadObjectInfo>& dataTable);

private:

	ExternalFile();

	ExternalFile(const ExternalFile&) = delete;
	void operator = (const ExternalFile&) = delete;

	PlayerInfo player_ = {};		//プレイヤーのステータス情報

	std::string startPosName_ = {};	//開始位置の名前

	std::deque<int> pastTotalDeathNum_ = {};

	std::unordered_map<std::string, int> loadFile_;										//ロードしたファイル

	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadMainStageObjInfo_;	//メインステージオブジェクトの配置データ
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadOpeningStageObjInfo_;//オープニングステージオブジェクトの配置データ
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadEndStageObjInfo_;	//オープニングステージオブジェクトの配置データ
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadGimmickInfo_;		//ギミックの配置データ	
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadCameraGimmickInfo_;	//カメラギミックの配置データ		
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadEnemyInfo_;			//敵の配置データ

	std::unordered_map<std::string, LoadObjectInfo>			   loadTutorialInfo_;		//チュートリアルポイントの配置データ	
	std::unordered_map<std::string, LoadObjectInfo>			   loadStartPos_;			//開始位置のデータ	
	std::unordered_map<std::string, LoadObjectInfo>			   loadCameraPosInfo_;		//カメラの配置データ	
	std::unordered_map<std::string, LoadObjectInfo>			   loadUIInfo_;				//UIの配置データ	
};

