#pragma once
#include "../object/PlayerData.h"
#include "../object/ObjectData.h"
#include <list>
#include <deque>
#include <string>
#include <DxLib.h>
#include <unordered_map>

//分割画像の構造体
struct DivGraphData
{
	std::string name = {};	//名前

	int divXNum = 0;		//画像のXの分割数
	int divYNum = 0;		//画像のYno分割数

	int divXSize = 0;		//画像のXの分割サイズ
	int divYSize = 0;		//画像のYの分割サイズ
};

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
	void LoadFileHandle(const std::string& name);

	/// <summary>
	/// セーブデータの書き出し
	/// </summary>
	/// <param name="num">死亡回数</param>
	void SaveDataRewriteInfo(const int num);

	/// <summary>
	///	モデルファイルパスを読み込む
	/// </summary>
	void LoadModelFilePath();

	/// <summary>
	/// 画像のファイルパスを読み込む
	/// </summary>
	void LoadGraphFilePath();

	/// <summary>
	/// 分割画像のファイルパスを読み込む
	/// </summary>
	void LoadDivGraphFilePath();


	////////////////Getter////////////////

	/// <summary>
	/// 特定のギミックの配置情報を取得する
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <returns></returns>
	LoadObjectInfo GetSpecifiedGimmickInfo(const std::string& name);

	/// <summary>
	/// カメラが特殊な動きを行う印(オブジェクト)が
	/// どこにあるかの配置データを取得する
	/// </summary>
	/// <returns></returns>
	LoadObjectInfo GetCameraGimmickInfo(const VECTOR& playerPos, const std::string& name);

	/// <summary>
	/// タイトルの指定した名前のオブジェクト配置データを返す
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <returns>オブジェクトのデータ</returns>
	LoadObjectInfo GetTitleSpecifiedInfo(const std::string& name);

	/// <summary>
	/// ゲームメイン指定した名前のオブジェクト配置データを返す
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <returns>オブジェクトのデータ</returns>
	LoadObjectInfo GetMainSpecifiedInfo(const std::string& name);

	/// <summary>
	/// エンディングシーンの指定した名前のオブジェクト配置データを返す
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <returns>オブジェクトのデータ</returns>
	LoadObjectInfo GetEndSpecifiedInfo(const std::string& name);

	/// <summary>
	/// エネミーの配置データを取得する
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <returns>配置データ</returns>
	std::list<LoadObjectInfo> GetEnemyInfo(const VECTOR& playerPos);

	/// <summary>
	/// プレイヤーの開始位置のデータを取得する
	/// </summary>
	/// <param name="name">開始位置の名前</param>
	/// <returns>配置データ</returns>
	VECTOR GetStartPos(const std::string& name);

	/// <summary>
	/// チュートリアルを表示するポイントの配置データを取得する
	/// </summary>
	/// <param name="pos">プレイヤーのポジション</param>
	/// <returns>配置データ</returns>
	LoadObjectInfo GetTutorialObjInfo(const VECTOR& pos);

	/// <summary>
	/// カメラの座標データを取得する
	/// </summary>
	/// <param name="name">名前</param>
	/// <returns>座標データ</returns>
	VECTOR GetCameraTargetPos(const std::string& name);

	/// <summary>
	/// 指定UIの配置座標を取得する
	/// </summary>
	/// <param name="name">取得したいUIの名前</param>
	/// <returns>座標</returns>
	VECTOR GetUIPos(const std::string& name);

	/// <summary>
	/// プレイヤーに関する情報を取得する
	/// </summary>
	/// <returns>playerData</returns>
	PlayerInfo GetPlayerInfo() const { return player_; }

	/// <summary>
	/// メインステージの特殊なオブジェクトの配置データを取得する
	/// </summary>
	/// <param name="name">名前</param>
	/// <returns>配置データ</returns>
	LoadObjectInfo GetMainSpecialObjectInfo(const std::string& name) { return loadSpecialInfo_[name]; }

	/// <summary>
	/// 開始場所の名前を取得する
	/// </summary>
	/// <returns>開始場所の名前</returns>
	const std::string& GetStartName()const { return startPosName_; }

	/// <summary>
	/// savedataを取得する
	/// </summary>
	/// <returns>過去の死亡回数</returns>
	const std::deque<int>& GetTotalDeathNum() const { return pastTotalDeathNum_; }

	/// <summary>
	/// オブジェクトの配置情報を取得
	/// </summary>
	/// <returns>配置データ</returns>
	const std::unordered_map<std::string, std::list<LoadObjectInfo>>& GetGimmickInfo() const { return loadGimmickInfo_; }

	/// <summary>
	/// メインステージオブジェクトの配置、回転率、拡縮率を取得する
	/// </summary>
	/// <returns>オブジェクトの配置データなどをまとめた変数</returns>
	const std::unordered_map<std::string, std::list<LoadObjectInfo>>& GetLoadMainStageObjectInfo()const { return loadMainStageObjInfo_; }

	/// <summary>
	/// オープニングステージオブジェクトの配置、回転率、拡縮率を取得する
	/// </summary>
	/// <returns>オブジェクトの配置データなどをまとめた変数</returns>
	const std::unordered_map<std::string, std::list<LoadObjectInfo>>& GetLoadOpeningStageObjectInfo() const { return loadOpeningStageObjInfo_; }

	/// <summary>
	/// エンディングシーンオブジェクトの配置、回転率、拡縮率を取得する
	/// </summary>
	/// <returns>オブジェクトの配置データなどをまとめた変数</returns>
	const std::unordered_map<std::string, std::list<LoadObjectInfo>>& GetLoadEndingStageObjectInfo()const { return loadEndStageObjInfo_; }

	/// <summary>
	/// モデルファイルパスを取得する
	/// </summary>
	/// <returns>ファイルパスをまとめたテーブル</returns>
	const std::unordered_map<std::string, std::list<std::string>>& GetModelFilePath()const { return modelFilePathInfo_; }

	/// <summary>
	/// 画像ファイルパスを取得する
	/// </summary>
	/// <returns>ファイルパスをまとめたテーブル</returns>
	const std::unordered_map<std::string, std::list<std::string>>& GetGraphFilePath()const { return graphFilePathInfo_; }

	/// <summary>
	/// 分割データを取得する
	/// </summary>
	/// <returns>分割データをまとめたテーブル</returns>
	const std::unordered_map<std::string, std::list<DivGraphData>>& GetDivGraphData()const { return divGraphFilePathInfo_; }


	////////////////Setter////////////////

	/// <summary>
	/// 開始場所の名前を設定する
	/// </summary>
	/// <param name="name">開始場所の名前</param>
	void SetStartName(const std::string& name);

	/// <summary>
	/// 死んだ回数をセットする
	/// </summary>
	/// <param name="num"></param>
	void SetDeathCount(const int num);
private:

	/// <summary>
	/// プレイヤーのステータス情報を書き出す 
	/// </summary>
	void RewritePlayerInfo();

	/// <summary>
	/// プレイヤーのステータスに関する情報を読み込む
	/// </summary>
	void LoadPlayerInfo();

	/// <summary>
	/// セーブデータを読み込む
	/// </summary>
	/// <param name="filename">ファイルの名前</param>
	void LoadSaveDataInfo(const std::string& filename);

	/// <summary>
	/// オブジェクトの配置情報を読み込みlistに格納する
	/// </summary>
	/// <param name="filename">ファイルの名前</param>
	/// <param name="dataTable">データテーブル</param>
	void LoadObjectDataList(const std::string& name,std::unordered_map<std::string, std::list<LoadObjectInfo>>& dataTable);

	/// <summary>
	/// オブジェクトの配置情報を読み込む
	/// </summary>
	/// /// <param name="filename">ファイルの名前</param>
	/// <param name="dataTable">データテーブル</param>
	void LoadObjectData(const std::string& name,std::unordered_map<std::string, LoadObjectInfo>& dataTable);
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ExternalFile();

	ExternalFile(const ExternalFile&) = delete;
	void operator = (const ExternalFile&) = delete;
private:
	PlayerInfo player_ = {};																//プレイヤーのステータス情報

	std::string startPosName_ = {};															//開始位置の名前

	std::deque<int> pastTotalDeathNum_ = {};												//過去5回の死亡数

	std::unordered_map<std::string, int> loadFile_;											//ロードしたファイル
	std::unordered_map<std::string, std::list<std::string>>		modelFilePathInfo_;			//モデルファイルパスをまとめる
	std::unordered_map<std::string, std::list<std::string>>		graphFilePathInfo_;			//画像ファイルパスをまとめる
	std::unordered_map<std::string, std::list<DivGraphData>>	divGraphFilePathInfo_;		//分割画像ファイルパスをまとめる

	std::unordered_map<std::string, std::list<LoadObjectInfo>>	loadMainStageObjInfo_;		//メインステージオブジェクトの配置データ
	std::unordered_map<std::string, std::list<LoadObjectInfo>>	loadOpeningStageObjInfo_;	//オープニングステージオブジェクトの配置データ
	std::unordered_map<std::string, std::list<LoadObjectInfo>>	loadEndStageObjInfo_;		//オープニングステージオブジェクトの配置データ
	std::unordered_map<std::string, std::list<LoadObjectInfo>>	loadGimmickInfo_;			//ギミックの配置データ	
	std::unordered_map<std::string, std::list<LoadObjectInfo>>	loadCameraGimmickInfo_;		//カメラギミックの配置データ		
	std::unordered_map<std::string, std::list<LoadObjectInfo>>	loadEnemyInfo_;				//敵の配置データ
	
	std::unordered_map<std::string, LoadObjectInfo>				loadUIInfo_;				//UIの配置データ	
	std::unordered_map<std::string, LoadObjectInfo>				loadStartPos_;				//開始位置のデータ	
	std::unordered_map<std::string, LoadObjectInfo>				loadSpecialInfo_;			//特殊なオブジェクトの配置データ	
	std::unordered_map<std::string, LoadObjectInfo>				loadTutorialInfo_;			//チュートリアルポイントの配置データ	
	std::unordered_map<std::string, LoadObjectInfo>				loadCameraPosInfo_;			//カメラの配置データ	
};

